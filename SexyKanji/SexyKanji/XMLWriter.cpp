// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
// XMLWriter.cpp: implementation of the XMLWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "XMLWriter.h"
#include "KTextFace.h"

// Last include directive:
#include "memmgr.h"

#pragma warning(disable : 4018)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace Sexy;


/*!***********************************
// \brief		
// \details 	<b>Description:</b> 
//************************************/
XMLWriter::XMLWriter()
{
	mLineNum = 0;
	mAllowComments = false;
	mOpenAttributes = false;
	mBufferWriteIndex = 0;
	memset(mWriteBuffer, 0, MAX_XML_WRITER_BUFFER_SIZE);

	// Init buffer with UTF-8 BOM (JPOAG)
	KTextFace::encodeChar(K_UTF8, (char*)mWriteBuffer, mBufferWriteIndex, 0xFEFF);

	mFileClosed= false;
}

/*!***********************************
// \brief		
// \details 	<b>Description:</b> 
//************************************/
XMLWriter::~XMLWriter()
{
	CloseFile();
}

/*!***********************************
// @return    	void
// @param     	theErrorText
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void XMLWriter::Fail(const SexyString& theErrorText)
{
	mHasFailed = true;
	mErrorText = theErrorText;
}
/*!***********************************
// @return    	void
// @param     	theWarning
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void XMLWriter::Warn(const SexyString &theWarning)
{
	mWarningStack.push(_S("WARNING: ") + theWarning);
}
/*!***********************************
// @return    	void
// @param     	theComment
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void XMLWriter::Comment(const SexyString &theComment)
{
	mWarningStack.push(theComment);
}
/*!***********************************
// @return    	void
//
// \brief		Called Internally
// \details 	<b>Description:</b> Called Internally
//************************************/
void XMLWriter::Init()
{
	mLineNum = 1;
	mHasFailed = false;	
}

/*!***********************************
// @return    	bool
// @param     	theFileName
//
// \brief		Open the Specified File for Writing.
// \details 	<b>Description:</b> File are saved in UTF-8 Format with BOM.  If you want something different,
//				download Notepad++ <a href="http://notepad-plus.sourceforge.net/">http://notepad-plus.sourceforge.net/</a>
//************************************/
bool XMLWriter::OpenFile(const SexyString& theFileName)
{		
	mFileName = theFileName.c_str();
	Init();
	return true;
}

/*!***********************************
// @return    	bool
//
// \brief		Returns true when writing has failed.
// \details 	<b>Description:</b> This rarely happens.
//************************************/
bool XMLWriter::HasFailed()
{
	return mHasFailed;
}

/*!***********************************
// @return    	SexyString
//
// \brief		The Error Text of what went wrong when writing the XML.
// \details 	<b>Description:</b> The Error Text of what went wrong when writing the XML.
//************************************/
SexyString XMLWriter::GetErrorText()
{
	return mErrorText;
}

/*!***********************************
// @return    	int
//
// \brief		Not used.
// \details 	<b>Description:</b> Invalid.
//************************************/
int XMLWriter::GetCurrentLineNum()
{
	return mLineNum;
}

/*!***********************************
// @return    	SexyString
//
// \brief		Returns the Filename the buffer will be written to.
// \details 	<b>Description:</b> Returns the Filename the buffer will be written to.
//************************************/
SexyString XMLWriter::GetFileName()
{
	return mFileName;
}

/*!***********************************
// @return    	bool
// @param     	theElement
// @param     	theAttributeKey
// @param     	theAttributeValue
//
// \brief		Used as a tool to add attributes to XMLElement Nodes.
// \details 	<b>Description:</b> Used as a tool to add attributes to XMLElement Nodes.
//************************************/
bool XMLWriter::AddAttribute(XMLElement* theElement, const SexyString& theAttributeKey, const SexyString& theAttributeValue)
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
// @param     	theElementName
//
// \brief		Start a new Element: <ElementName 
// \details 	<b>Description:</b> Pushes the Element onto the section stack and creates 
a new Node with an Attributes section
//************************************/
bool XMLWriter::StartElement(const SexyString &theElementName)
{
	CheckFileOpen();
	if(mHasFailed) return false;

	if(mOpenAttributes)
	{
		// Close Previous Element Attribute section
		WriteStringToBuffer(_S(">\n"));
		mLineNum++;
	}

	if(!ValidateElementNodeName(theElementName))
	{
		Warn(theElementName + _S(" is an Invalid Node Name."));
	}

	while(!mWarningStack.empty())
	{
		WriteStringToBuffer(StrFormat(_S("<!--  %s -->\n"), mWarningStack.top().c_str()));
		mWarningStack.pop();
	}

	mSectionStack.push(theElementName);
	
	for(int i = 1; i < mSectionStack.size(); i++)
	{
		WriteStringToBuffer(L"\t");
	}
	
	WriteStringToBuffer(StrFormat(_S("<%s"), theElementName.c_str()));
	
	mOpenAttributes = true;
	return true;
}

/*!***********************************
// @return    	bool
// @param     	theText
//
// \brief		Writes Text Between element Tags <Element>Text</Element>
// \details 	<b>Description:</b> This will stop any attribute writing and close the Element Tag: </Element>
// \warning		Warning: This Stops the Element!
//************************************/
bool XMLWriter::WriteElementText(SexyString theText)
{
	CheckFileOpen();
	if(mHasFailed) return false;

	if(mOpenAttributes)
	{
		// Close Previous Element Attribute section
		WriteStringToBuffer(_S(">"));
		mOpenAttributes = false;
	}

	const SexyString aNodeName = mSectionStack.top();
	mSectionStack.pop();

	try
	{
		// I fixed this error, but just in case, I handle it by re-encoding it
		WriteStringToBuffer(StrFormat(_S("%s"), theText.c_str()));
	}
	catch(...)
	{
		WriteStringToBuffer(StrFormat(_S("%s"), XMLEncodeString(theText).c_str()));
	}
	WriteStringToBuffer(StrFormat(_S("</%s>\n"), aNodeName.c_str()));

	mLineNum++;

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theElement
//
// \brief		Starts a Next Element
// \details 	<b>Description:</b> Pushes theElement->Value onto the stack and adds all of 
theElements->mAttributes to the Attribute section
//************************************/
bool XMLWriter::StartElement(XMLElement *theElement)
{
	if(StartElement(theElement->mValue.c_str()) == false)
		return false;

	std::map<SexyString, SexyString>::iterator map_itr;
	map_itr = theElement->mAttributes.begin();

	for( ; map_itr != theElement->mAttributes.end(); map_itr++)
	{
		if(!WriteAttribute(map_itr->first, map_itr->second))
			return false;
	}
	
	return true;
}

/*!***********************************
// @return    	bool
//
// \brief		Closes The previously opened Element.  LIFO
// \details 	<b>Description:</b> Closes the previously open Node and pops it from the stack.
Also Closes the Attributes Writing until StartElement is called.
//************************************/
bool XMLWriter::StopElement()
{
	CheckFileOpen();
	if(mHasFailed) return false;

	if(mSectionStack.empty())
	{
		Fail(_S("Stop Element Calls do not match StartElement Calls."));
		return false;
	}

	const SexyString aNodeName = mSectionStack.top();
	mSectionStack.pop();

	if(mOpenAttributes)
	{
		// Close Previous Element Attribute section
		WriteStringToBuffer(L"/>\n");
		mLineNum++;
	}
	else
	{
		// Otherwise close element section
		for(int i = 0; i < mSectionStack.size(); i++)
		{
			WriteStringToBuffer(L"\t");
		}

		WriteStringToBuffer(StrFormat(L"</%s>\n", aNodeName.c_str()));
	}

	mOpenAttributes = false;

	while(!mWarningStack.empty())
	{
		WriteStringToBuffer(StrFormat(_S("<!--  %s -->\n"), mWarningStack.top().c_str()));
		mWarningStack.pop();
	}

	return true;
}

/*!***********************************
// @return    	bool
// @param     	aAttributeKey
// @param     	aAttributeValue
//
// \brief		Writes an attribute on the currently open element <Element attribute="value" 
// \details 	<b>Description:</b>Adds an attribute to the Current Element.  If No element is open, then it returns
//				false. 
//************************************/
bool XMLWriter::WriteAttribute(const SexyString& aAttributeKey, const SexyString& aAttributeValue)
{
	CheckFileOpen();
	if(mHasFailed) return false;

	if(mOpenAttributes)
	{
		if(!ValidateElementNodeName(aAttributeKey))
		{
			Warn(aAttributeKey + _S(" is an invalid Attribute Name."));
		}
		
		WriteStringToBuffer(StrFormat(_S(" %s=\"%s\""), aAttributeKey.c_str(), XMLEncodeString(aAttributeValue).c_str()));
		return true;
	}

	if(mSectionStack.size())
		Fail(_S("Attributes Section already closed for ") + mSectionStack.top());
	else
		Fail(_S("No Element Nodes Open for Writing Attributes."));

	return false;
}

/*!***********************************
// @return    	bool
// @param     	aAttributeKey
// @param     	aAttributeValue
//
// \brief		Writes an attribute on the currently open element <Element attribute="value" 
// \details 	<b>Description:</b>Adds an attribute to the Current Element.  If No element is open, then it returns
//				false. 
//************************************/
bool XMLWriter::WriteAttribute(const SexyString &aAttributeKey, const int &aAttributeValue)
{
	return WriteAttribute(aAttributeKey, StrFormat(_S("%d"), aAttributeValue));
}

/*!***********************************
// @return    	bool
// @param     	aAttributeKey
// @param     	aAttributeValue
//
// \brief		Writes an attribute on the currently open element <Element attribute="value" 
// \details 	<b>Description:</b>Adds an attribute to the Current Element.  If No element is open, then it returns
//				false. 
//************************************/
bool XMLWriter::WriteAttribute(const SexyString &aAttributeKey, const float &aAttributeValue)
{
	return WriteAttribute(aAttributeKey, StrFormat(_S("%f"), aAttributeValue));
}

/*!***********************************
// @return    	bool
//
// \brief		Writes the XML Stream to Disk.
// \details 	<b>Description:</b> The XML is encoded to UTF-8 and storred in a Sexy::Buffer.  This
//				call closes the buffer and writes it to disk.
//************************************/
bool XMLWriter::CloseFile()
{
	if(mFileClosed) return true;
	mFileClosed = true;

	while(!mSectionStack.empty())
	{
		StopElement();
	}

	Flush();

	if(mFileName != _S(""))
	{
		return Buffer::WriteBufferToFile(mFileName, mFileBuffer);
	}

	Fail(_S("File not Open"));
	return false;
}
/*!***********************************
// @return    	bool
// @param     	theNodeName
//
// \brief		Checks the Node Name.  
// \details 	<b>Description:</b> Only ANSI alphanumeric characters pass this test.
//************************************/
bool XMLWriter::ValidateElementNodeName(const SexyString &theNodeName)
{
	const SexyChar* aNodeName = theNodeName.c_str();

	for(int i = 0; i < theNodeName.size(); i++)
	{
		if( aNodeName[i] < '0'
			|| (aNodeName[i] > '9' && aNodeName[i] < 'A')
			|| (aNodeName[i] > 'Z' && aNodeName[i] < '_')
			|| (aNodeName[i] > '_' && aNodeName[i] < 'a')
			|| aNodeName[i] > 'z')
		{
			return false;
		}
	}
	return true;
}

/*!***********************************
// @return    	bool
//
// \brief		Checks to see if there is a file that we are writing to.
// \details 	<b>Description:</b> Checks to see if there is a file that we are writing to.
//************************************/
bool XMLWriter::CheckFileOpen()
{
	return mFileName != _S("");
}

/*!***********************************
// @return    	bool
// @param     	aAttributeKey
//
// \brief		Writes an attribute on the currently open element <Element attribute
// \details 	<b>Description:</b>Adds an attribute to the Current Element.  If No element is open, then it returns
//				false. 
//************************************/
bool Sexy::XMLWriter::WriteAttribute( const SexyString& aAttributeKey )
{
	CheckFileOpen();
	if(mHasFailed) return false;

	if(mOpenAttributes)
	{
		if(!ValidateElementNodeName(aAttributeKey))
		{
			Warn(aAttributeKey + _S(" is an invalid Attribute Name."));
		}

		WriteStringToBuffer(StrFormat(_S(" %s"), aAttributeKey.c_str()));
		return true;
	}

	if(mSectionStack.size())
		Fail(_S("Attributes Section already closed for ") + mSectionStack.top());
	else
		Fail(_S("No Element Nodes Open for Writing Attributes."));

	return false;
}

/*!***********************************
// @return    	void
//
// \brief		Internal Method. 
// \details 	<b>Description:</b> UTF-8 char buffer is full, flush it to the Sexy::Buffer
//************************************/
void Sexy::XMLWriter::Flush()
{
	mFileBuffer.WriteBytes(mWriteBuffer, mBufferWriteIndex);
	mBufferWriteIndex = 0;
}

/*!***********************************
// @return    	void
//
// \brief		Internal Method.
// \details 	<b>Description:</b> See if we are coming up on the end of the UTF-8 encoding space. Used internally, do not call this method.
//************************************/
void Sexy::XMLWriter::CheckWriteBuffer()
{
	if (mBufferWriteIndex + 4 >= MAX_XML_WRITER_BUFFER_SIZE) Flush();
}

/*!***********************************
// @return    	void
// @param     	theString
//
// \brief		Internal Method.  
// \details 	<b>Description:</b> Encode in UTF-8 and write to the Sexy::Buffer.
//************************************/
void Sexy::XMLWriter::WriteStringToBuffer( std::string theString )
{
	WriteStringToBuffer(StringToWString(theString));
}

/*!***********************************
// @return    	void
// @param     	theString
//
// \brief		Internal Method.  
// \details 	<b>Description:</b> Encode in UTF-8 and write to the Sexy::Buffer.
//************************************/
void Sexy::XMLWriter::WriteStringToBuffer( std::wstring theString )
{
	for (unsigned int i = 0; i < theString.length(); i++)
	{
		CheckWriteBuffer();
		KTextFace::encodeChar(K_UTF8, (char*)mWriteBuffer, mBufferWriteIndex, theString[i]);
	}
}