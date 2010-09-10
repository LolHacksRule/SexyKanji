// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "XMLParser.h"
#include "Buffer.h"
#include "Common.h"

#include "KTextFace.h"

#include <stack>

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
//
// \brief		Creates a new Parser Object.
// \details 	<b>Description:</b> This only allocates the object.  The setup takes place when you open a file.
//************************************/
XMLParser::XMLParser()
{
	mLineNum = 0;
	mAllowComments = false;
	mForcedEncodingType = false;
}

XMLParser::~XMLParser()
{
}

/*!***********************************
// @return    	bool
// @param     	theErrorText
//
// \brief		Internal Method.
// \details 	<b>Description:</b> Sets the error text and HasFailed Boolean.  Returns false always.
//************************************/
bool XMLParser::Fail(const SexyString& theErrorText)
{
	mHasFailed = true;
	mErrorText = theErrorText;
	return false;
}

/*!***********************************
// @return    	void
//
// \brief		Internal Method.
// \details 	<b>Description:</b> Resets the parser.
//************************************/
void XMLParser::Init()
{
	mSection = _S("");
	mLineNum = 1;
	mHasFailed = false;
	mErrorText = _S("");
	mFirstChar = true;
	mByteSwap = false;
}

/*!***********************************
// @return    	bool
// @param     	theElement
// @param     	theAttributeKey
// @param     	theAttributeValue
//
// \brief		Internal Method.
// \details 	<b>Description:</b> Adds an Attribute to the Element.
//************************************/
bool XMLParser::AddAttribute(XMLElement* theElement, const SexyString& theAttributeKey, const SexyString& theAttributeValue)
{
	std::pair<XMLParamMap::iterator,bool> aRet;

	aRet = theElement->mAttributes.insert(XMLParamMap::value_type(theAttributeKey, theAttributeValue));
	if (!aRet.second)
		aRet.first->second = theAttributeValue;

	if (theAttributeKey != _S("/"))
		theElement->mAttributeIteratorList.push_back(aRet.first);

	return aRet.second;
}
/*!***********************************
// @return    	bool
// @param     	theFileName
//
// \brief		Open File: Detect Encoding: Buffer Text
// \details 	<b>Description:</b> This method will detect the character encoding of the XML file and 
//				attempt to read the file into memory.  The result is parsed using NextElement()
//************************************/
bool XMLParser::OpenFile(const std::wstring& theFileName)
{		
	Buffer aFileBuffer;

	if (Buffer::ReadBufferFromFile(WStringToSexyStringFast(theFileName), aFileBuffer) && aFileBuffer.GetDataLen() > 0)
	{
		mFileName = WStringToString(theFileName);
		if (!mForcedEncodingType)
		{
			long anEncoding = K_ANSI;
			const unsigned char* aDataPtr = aFileBuffer.GetDataPtr();

			KTextFace::detectEncoding((const char*)aDataPtr, (long)aFileBuffer.GetDataLen(), anEncoding);

			//////////////////////////////////////////////////////////////////////////
			// Copy to Internal Buffer
			//////////////////////////////////////////////////////////////////////////
			std::stack<wchar_t>  aBufferStack; // mBufferedText is backward
			Init();

			wchar_t aTempChar = 0;
			long aCursor = 0;

			//////////////////////////////////////////////////////////////////////////
			// If Kanji does it, then use kanji! (JPOAG)
			//////////////////////////////////////////////////////////////////////////
			do 
			{
				aTempChar = (wchar_t)KTextFace::decodeChar(anEncoding, (const char*)aDataPtr, aCursor, true);
				if(aTempChar != 0 && aTempChar != 0xFEFF)
					aBufferStack.push(aTempChar);
			} while (aTempChar && aCursor < aFileBuffer.GetDataLen());

			//////////////////////////////////////////////////////////////////////////
			// The parser uses a vector like a stack, so the string needs to be reversed
			/////////////////////////////////////////////////////////////////(JPOAG)//
			mBufferedText.clear();
			while(!aBufferStack.empty())
			{
				mBufferedText.push_back(aBufferStack.top());
				aBufferStack.pop();
			}
		}
	}
	else
	{
		mLineNum = 0;
		return Fail(StringToSexyStringFast("Unable to open file ") + WStringToSexyStringFast(theFileName));
	}

	return !mBufferedText.empty();
}
/*!***********************************
// @return    	bool
// @param     	theFilename
//
// \brief		Opens the XML file.
// \details 	<b>Description:</b> Open the file and buffers the text.
//************************************/
bool Sexy::XMLParser::OpenFile( const std::string& theFilename )
{
	std::wstring aFileName = StringToWString(theFilename);
	return OpenFile(aFileName);
}

/*!***********************************
// @return    	void
// @param     	theString
//
// \brief		Sets the Source of the XML parser.
// \details 	<b>Description:</b> Sets the Source of the XML parser.
//************************************/
void XMLParser::SetStringSource(const std::wstring& theString)
{
	Init();

	int aSize = theString.size();

	mBufferedText.resize(aSize);	
	for (int i = 0; i < aSize; i++)
		mBufferedText[i] = theString[aSize - i - 1];	
}

/*!***********************************
// @return    	void
// @param     	theString
//
// \brief		Sets the Source of the XML parser.
// \details 	<b>Description:</b> Sets the Source of the XML parser.
//************************************/
void XMLParser::SetStringSource(const std::string& theString)
{
	SetStringSource(StringToWString(theString));
}

/*!***********************************
// @return    	bool
// @param     	theElement
//
// \brief		Parse the Next Element Node from the Stream.
// \details 	<b>Description:</b> Element Nodes are extracted from the stream one at a time.
//************************************/
bool XMLParser::NextElement(XMLElement* theElement)
{
	for (;;)
	{		
		theElement->mType = XMLElement::TYPE_NONE;
		theElement->mSection = mSection;
		theElement->mValue = _S("");
		theElement->mAttributes.clear();			
		theElement->mInstruction.erase();

		bool hasSpace = false;	
		bool inQuote = false;
		bool gotEndQuote = false;

		bool doingAttribute = false;
		bool AttributeVal = false;
		std::wstring aAttributeKey;
		std::wstring aAttributeValue;

		std::wstring aLastAttributeKey;
		
		for (;;)
		{
			// Process character by character

			wchar_t c;
			int aVal;
			
			if (mBufferedText.size() > 0)
			{								
				c = mBufferedText[mBufferedText.size()-1];
				mBufferedText.pop_back();				

				aVal = 1;
			}
			else
				aVal = 0;
			
			if (aVal == 1)
			{
				bool processChar = false;

				if (c == L'\n')
				{
					mLineNum++;
				}

				if (theElement->mType == XMLElement::TYPE_COMMENT)
				{
					// Just add text to theElement->mInstruction until we find -->

					SexyString* aStrPtr = &theElement->mInstruction;
					
					*aStrPtr += (SexyChar)c;					

					int aLen = aStrPtr->length();

					if ((c == L'>') && (aLen >= 3) && ((*aStrPtr)[aLen - 2] == L'-') && ((*aStrPtr)[aLen - 3] == L'-'))
					{
						*aStrPtr = aStrPtr->substr(0, aLen - 3);
						break;
					}
				}
				else if (theElement->mType == XMLElement::TYPE_INSTRUCTION)
				{
					// Just add text to theElement->mInstruction until we find ?>

					SexyString* aStrPtr = &theElement->mValue;

					if ((theElement->mInstruction.length() != 0) || (::iswspace(c)))
						aStrPtr = &theElement->mInstruction;
					
					*aStrPtr += (SexyChar)c;					

					int aLen = aStrPtr->length();

					if ((c == L'>') && (aLen >= 2) && ((*aStrPtr)[aLen - 2] == L'?'))
					{
						*aStrPtr = aStrPtr->substr(0, aLen - 2);
						break;
					}
				}
				else
				{
					if (c == L'"')
					{
						inQuote = !inQuote;
						if (theElement->mType==XMLElement::TYPE_NONE || theElement->mType==XMLElement::TYPE_ELEMENT)
							processChar = true;

						if (!inQuote)
							gotEndQuote = true;
					}
					else if (!inQuote)
					{
						if (c == L'<')
						{
							if (theElement->mType == XMLElement::TYPE_ELEMENT)
							{
								//TODO: Fix buffered text.  Not sure what I meant by that.

								//OLD: mBufferedText = c + mBufferedText;

								mBufferedText.push_back(c);								
								break;
							}

							if (theElement->mType == XMLElement::TYPE_NONE)
							{
								theElement->mType = XMLElement::TYPE_START;
							}
							else
							{
								Fail(_S("Unexpected '<'"));
								return false;
							}
						}
						else if (c == L'>')
						{
							if (theElement->mType == XMLElement::TYPE_START)
							{	
								bool insertEnd = false;

								if (aAttributeKey == L"/")
								{
									// We will get this if we have a space before the />, so we can ignore it
									//  and go about our business now
									insertEnd = true;
								}
								else
								{
									// Probably isn't committed yet
									if (aAttributeKey.length() > 0)
									{										
//										theElement->mAttributes[aLastAttributeKey] = aAttributeValue;

										aAttributeKey = XMLDecodeString(aAttributeKey);
										aAttributeValue = XMLDecodeString(aAttributeValue);

										aLastAttributeKey = aAttributeKey;
										AddAttribute(theElement, WStringToSexyStringFast(aLastAttributeKey), WStringToSexyStringFast(aAttributeValue));

										aAttributeKey = L"";
										aAttributeValue = L"";
									}

									if (aLastAttributeKey.length() > 0)
									{
										SexyString aVal = theElement->mAttributes[WStringToSexyStringFast(aLastAttributeKey)];

										int aLen = aVal.length();

										if ((aLen > 0) && (aVal[aLen-1] == _S('/')))
										{
											// Its an empty element, fake start and end segments
//											theElement->mAttributes[aLastAttributeKey] = aVal.substr(0, aLen - 1);
											
											AddAttribute(theElement, WStringToSexyStringFast(aLastAttributeKey), (XMLDecodeString(aVal.substr(0, aLen - 1))));

											insertEnd = true;
										}
									}
									else
									{
										int aLen = theElement->mValue.length();

										if ((aLen > 0) && (theElement->mValue[aLen-1] == '/'))
										{
											// Its an empty element, fake start and end segments
											theElement->mValue = theElement->mValue.substr(0, aLen - 1);
											insertEnd = true;
										}
									}
								}

								// Do we want to fake an ending section?
								if (insertEnd)
								{									
									std::wstring anAddString = L"</" + SexyStringToWStringFast(theElement->mValue) + L">";

									int anOldSize = mBufferedText.size();
									int anAddLength = anAddString.length();

									mBufferedText.resize(anOldSize + anAddLength);

									for (int i = 0; i < anAddLength; i++)
										mBufferedText[anOldSize + i] = (wchar_t)(anAddString[anAddLength - i - 1]);

									// clear out aAttributeKey, since it contains "/" as its value and will insert
									// it into the element's attribute map.
									aAttributeKey = L"";

									//OLD: mBufferedText = "</" + theElement->mValue + ">" + mBufferedText;
								}

								if (mSection.length() != 0)
									mSection += _S("/");

								mSection += theElement->mValue;								

								break;
							}
							else if (theElement->mType == XMLElement::TYPE_END)
							{
								int aLastSlash = mSection.rfind(_S('/'));
								if ((aLastSlash == -1) && (mSection.length() == 0))
								{
									Fail(_S("Unexpected End"));
									return false;
								}

								SexyString aLastSectionName = mSection.substr(aLastSlash + 1);
								
								if (aLastSectionName != theElement->mValue)
								{
									Fail(_S("End '") + theElement->mValue + _S("' Doesn't Match Start '") + aLastSectionName + _S("'"));
									return false;
								}

								if (aLastSlash == -1)
									mSection.erase(mSection.begin(), mSection.end());
								else
									mSection.erase(mSection.begin() + aLastSlash, mSection.end());

								break;
							}
							else
							{
								Fail(_S("Unexpected '>'"));
								return false;
							}
						}
						else if ((c == L'/') && (theElement->mType == XMLElement::TYPE_START) && (theElement->mValue == _S("")))
						{					
							theElement->mType = XMLElement::TYPE_END;					
						}				
						else if ((c == L'?') && (theElement->mType == XMLElement::TYPE_START) && (theElement->mValue == _S("")))
						{
							theElement->mType = XMLElement::TYPE_INSTRUCTION;
						}
						else if (::isspace((unsigned char) c))
						{
							if (theElement->mValue != _S(""))
								hasSpace = true;

							// It's a comment!
							if ((theElement->mType == XMLElement::TYPE_START) && (theElement->mValue == _S("!--")))
								theElement->mType = XMLElement::TYPE_COMMENT;
						}
						else if (c > 32)
						{
							processChar = true;
						}
						else
						{
							Fail(_S("Illegal Character"));
							return false;
						}
					} 
					else
					{
						processChar = true;
					}

					if (processChar)
					{
						if (theElement->mType == XMLElement::TYPE_NONE)
							theElement->mType = XMLElement::TYPE_ELEMENT;

						if (theElement->mType == XMLElement::TYPE_START)
						{
							if (hasSpace)
							{
								if ((!doingAttribute) || ((!AttributeVal) && (c != _S('='))) ||
									((AttributeVal) && ((aAttributeValue.length() > 0) || gotEndQuote)))
								{
									if (doingAttribute)
									{
										aAttributeKey = XMLDecodeString(aAttributeKey);
										aAttributeValue = XMLDecodeString(aAttributeValue);

//										theElement->mAttributes[aAttributeKey] = aAttributeValue;

										AddAttribute(theElement, WStringToSexyStringFast(aAttributeKey), WStringToSexyStringFast(aAttributeValue));

										aAttributeKey = L"";
										aAttributeValue = L"";

										aLastAttributeKey = aAttributeKey;
									}
									else
									{
										doingAttribute = true;
									}
																
									AttributeVal = false;
								}

								hasSpace = false;
							}

							std::wstring* aStrPtr = NULL;

							if (!doingAttribute)
							{
								theElement->mValue += (SexyChar)c;
							}
							else
							{
								if (!AttributeVal && c == L'=')
								{
									AttributeVal = true;
									gotEndQuote = false;
								}
								else
								{
									if (!AttributeVal)
										aStrPtr = &aAttributeKey;
									else
										aStrPtr = &aAttributeValue;
								}
							}

							if (aStrPtr != NULL)
							{								
								*aStrPtr += c;						
							}
						}
						else
						{
							if (hasSpace)
							{
								theElement->mValue += _S(" ");
								hasSpace = false;
							}
							
							theElement->mValue += (SexyChar)c;
						}
					}
				}
			}
			else
			{
				if (theElement->mType != XMLElement::TYPE_NONE)
					Fail(_S("Unexpected End of File"));
					
				return false;
			}			
		}		

		if (aAttributeKey.length() > 0)
		{
			aAttributeKey = XMLDecodeString(aAttributeKey);
			aAttributeValue = XMLDecodeString(aAttributeValue);
//			theElement->mAttributes[aAttributeKey] = aAttributeValue;

			AddAttribute(theElement, WStringToSexyStringFast(aAttributeKey), WStringToSexyStringFast(aAttributeValue));
		}

		theElement->mValue = (XMLDecodeString(theElement->mValue));				

		// Ignore comments
		if ((theElement->mType != XMLElement::TYPE_COMMENT) || mAllowComments)
			return true;
	}
}

/*!***********************************
// @return    	bool
//
// \brief		Returns true if there was an error opening/parsing the XML.
// \details 	<b>Description:</b> HasFailed() will return true and GetErrorText() will be the description.
//************************************/
bool XMLParser::HasFailed()
{
	return mHasFailed;
}

/*!***********************************
// @return    	SexyString
//
// \brief		Return the Textual description of any errors that have occurred.
// \details 	<b>Description:</b> HasFailed() will return true and GetErrorText() will be the description.
//************************************/
SexyString XMLParser::GetErrorText()
{
	return mErrorText;
}
/*!***********************************
// @return    	int
//
// \brief		Return the current line number.
// \details 	<b>Description:</b> Return the current line number.
//************************************/
int XMLParser::GetCurrentLineNum()
{
	return mLineNum;
}
/*!***********************************
// @return    	std::string
//
// \brief		Return the name of the file being parsed.
// \details 	<b>Description:</b> Return the name of the file being parsed.
//************************************/
std::string XMLParser::GetFileName()
{
	return mFileName;
}

/*!***********************************
// @return    	bool
// @param     	theNode
// @param     	theAttrbiute
//
// \brief		checks to see if the named attribute exists in the given node.
// \details 	<b>Description:</b> checks to see if the named attribute exists in the given node.
//************************************/
bool Sexy::XMLParser::HasAttribute( XMLElement* theNode, SexyString theAttrbiute )
{
	return (theNode != NULL && theNode->mAttributes.find(theAttrbiute) != theNode->mAttributes.end());
}

/*!***********************************
// @return    	std::string
// @param     	theString
//
// \brief		Converts an XML encoded string to a normal string
// \details 	<b>Description:</b> Converts an XML encoded string to a normal string
//************************************/
std::string Sexy::XMLParser::XMLDecodeString( const std::string& theString )
{
	std::string aNewString;

	int aUTF8Len = 0;
	int aUTF8CurVal = 0;

	for (unsigned long i = 0; i < theString.length(); i++)
	{
		char c = theString[i];

		if (c == '&')
		{
			int aSemiPos = theString.find(';', i);

			if (aSemiPos != -1)
			{
				std::string anEntName = theString.substr(i+1, aSemiPos-i-1);
				i = aSemiPos;

				if (anEntName == "lt")
					c = '<';
				else if (anEntName == "amp")
					c = '&';
				else if (anEntName == "gt")
					c = '>';
				else if (anEntName == "quot")
					c = '"';
				else if (anEntName == "apos")
					c = '\'';
				else if (anEntName == "nbsp")
					c = ' ';
				else if (anEntName == "cr")
					c = '\n';
			}
		}				

		aNewString += c;
	}

	return aNewString;
}

/*!***********************************
// @return    	std::wstring
// @param     	theString
//
// \brief		Converts an XMLEncoded string to a normal string
// \details 	<b>Description:</b> Converts an XMLEncoded string to a normal string
//************************************/
std::wstring Sexy::XMLParser::XMLDecodeString( const std::wstring& theString )
{
	std::wstring aNewString;

	int aUTF8Len = 0;
	int aUTF8CurVal = 0;

	for (unsigned long i = 0; i < theString.length(); i++)
	{
		wchar_t c = theString[i];

		if (c == L'&')
		{
			int aSemiPos = theString.find(L';', i);

			if (aSemiPos != -1)
			{
				std::wstring anEntName = theString.substr(i+1, aSemiPos-i-1);
				i = aSemiPos;

				if (anEntName == L"lt")
					c = L'<';
				else if (anEntName == L"amp")
					c = L'&';
				else if (anEntName == L"gt")
					c = L'>';
				else if (anEntName == L"quot")
					c = L'"';
				else if (anEntName == L"apos")
					c = L'\'';
				else if (anEntName == L"nbsp")
					c = L' ';
				else if (anEntName == L"cr")
					c = L'\n';
			}
		}				

		aNewString += c;
	}

	return aNewString;
}
/*!***********************************
// @return    	std::string
// @param     	theString
//
// \brief		Encodes a string for storage in XML attributes
// \details 	<b>Description:</b> Encodes a string for storage in XML attributes
//************************************/
std::string Sexy::XMLParser::XMLEncodeString( const std::string& theString )
{
	std::string aNewString;

	bool hasSpace = false;

	for (unsigned long i = 0; i < theString.length(); i++)
	{
		char c = theString[i];

		if (c == ' ')
		{
			if (hasSpace)
			{
				aNewString += "&nbsp;";
				continue;
			}

			hasSpace = true;
		}
		else
			hasSpace = false;

		/*if ((uchar) c >= 0x80)
		{
		// Convert to UTF
		aNewString += (char) (0xC0 | ((c >> 6) & 0xFF));
		aNewString += (char) (0x80 | (c & 0x3F));
		}
		else*/
		{		
			switch (c)
			{
			case '<':
				aNewString += "&lt;";
				break;
			case '&':		
				aNewString += "&amp;";
				break;
			case '>':
				aNewString += "&gt;";
				break;
			case '"':
				aNewString += "&quot;";
				break;
			case '\'':
				aNewString += "&apos;";
				break;
			case '\n':
				aNewString += "&cr;";
				break;
			default:
				aNewString += c;
				break;
			}
		}
	}

	return aNewString;
}
/*!***********************************
// @return    	std::wstring
// @param     	theString
//
// \brief		Encodes a string for storage in XML Attributes
// \details 	<b>Description:</b> Encodes a string for storage in XML Attributes
//************************************/
std::wstring Sexy::XMLParser::XMLEncodeString( const std::wstring& theString )
{
	std::wstring aNewString;

	bool hasSpace = false;

	for (unsigned long i = 0; i < theString.length(); i++)
	{
		wchar_t c = theString[i];

		if (c == ' ')
		{
			if (hasSpace)
			{
				aNewString += L"&nbsp;";
				continue;
			}

			hasSpace = true;
		}
		else
			hasSpace = false;

		/*if ((uchar) c >= 0x80)
		{
		// Convert to UTF
		aNewString += (char) (0xC0 | ((c >> 6) & 0xFF));
		aNewString += (char) (0x80 | (c & 0x3F));
		}
		else*/
		{		
			switch (c)
			{
			case L'<':
				aNewString += L"&lt;";
				break;
			case L'&':		
				aNewString += L"&amp;";
				break;
			case L'>':
				aNewString += L"&gt;";
				break;
			case L'"':
				aNewString += L"&quot;";
				break;
			case L'\'':
				aNewString += L"&apos;";
				break;
			case L'\n':
				aNewString += L"&cr;";
				break;
			default:
				aNewString += c;
				break;
			}
		}
	}

	return aNewString;
}