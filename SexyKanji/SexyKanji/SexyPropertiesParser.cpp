// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "SexyPropertiesParser.h"
#include "SexyProperties.h"
#include "XMLParser.h"
#include <stdlib.h>
#include "Buffer.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
// @param     	theProperties
//
// \brief		Pass in a pointer to the SexyProperties this parse is to populate.
// \details 	<b>Description:</b> Pass in a pointer to the SexyProperties this parse is to populate.
//************************************/
SexyPropertiesParser::SexyPropertiesParser(SexyProperties* theProperties)
{
	mProperties = theProperties;
	mHasFailed = false;
	mXMLParser = NULL;
}

/*!***********************************
// @return    	void
// @param     	theErrorText
//
// \brief		Internal method to throw an error string.
// \details 	<b>Description:</b> Internal method to throw an error string.  Use GetErrorText() to
//				view failure description.
//************************************/
void SexyPropertiesParser::Fail(const SexyString& theErrorText)
{
	if (!mHasFailed)
	{
		mHasFailed = true;
		int aLineNum = mXMLParser->GetCurrentLineNum();

		mError = theErrorText;
		if (aLineNum > 0) mError += StrFormat(_S(" on Line %d"), aLineNum);
		if (!mXMLParser->GetFileName().empty()) mError += StringToSexyStringFast(StrFormat((" in File '%s'"), mXMLParser->GetFileName().c_str()));

		K_LOG("Sexy::SexyPropertiesParser::Fail :Error: %s", SexyStringToStringFast(mError).c_str());
	}
}


/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyPropertiesParser::~SexyPropertiesParser()
{
	mProperties = NULL;
}


/*!***********************************
// @return    	bool
// @param     	aString
//
// \brief		Internal Parsing tool
// \details 	<b>Description:</b> Internal Parsing tool
//************************************/
bool SexyPropertiesParser::ParseSingleElement(SexyString* aString)
{
	*aString = _S("");

	for (;;)
	{
		XMLElement aXMLElement;
		if (!mXMLParser->NextElement(&aXMLElement))
			return false;
		
		if (aXMLElement.mType == XMLElement::TYPE_START)
		{
			Fail(_S("Unexpected Section: '") + aXMLElement.mValue + _S("'"));
			return false;			
		}
		else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
		{
			*aString = XMLDecodeString(aXMLElement.mValue);
		}		
		else if (aXMLElement.mType == XMLElement::TYPE_END)
		{
			return true;
		}
	}
}

/*!***********************************
// @return    	bool
// @param     	theStringVector
//
// \brief		Internal Parsing tool
// \details 	<b>Description:</b> Internal Parsing tool
//************************************/
bool SexyPropertiesParser::ParseStringArray(StringVector* theStringVector)
{
	theStringVector->clear();

	for (;;)
	{
		XMLElement aXMLElement;
		if (!mXMLParser->NextElement(&aXMLElement))
			return false;
		
		if (aXMLElement.mType == XMLElement::TYPE_START)
		{
			if (aXMLElement.mValue == _S("String"))
			{
				SexyString aString;

				if (!ParseSingleElement(&aString))
					return false;

				theStringVector->push_back((aString));
			}
			else
			{
				Fail(_S("Invalid Section '") + aXMLElement.mValue + _S("'"));
				return false;
			}
		}
		else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
		{
			Fail(_S("Element Not Expected '") + aXMLElement.mValue + _S("'"));
			return false;
		}		
		else if (aXMLElement.mType == XMLElement::TYPE_END)
		{
			return true;
		}
	}
}

/*!***********************************
// @return    	bool
//
// \brief		Internal Helper to check XML Elements for properties
// \details 	<b>Description:</b> Internal Helper to check XML Elements for properties
//************************************/
bool SexyPropertiesParser::ParseProperties()
{
	for (;;)
	{
		XMLElement aXMLElement;
		if (!mXMLParser->NextElement(&aXMLElement))
			return false;
		
		if (aXMLElement.mType == XMLElement::TYPE_START)
		{
			if (aXMLElement.mValue == _S("String"))
			{				
				SexyString aDef;
				if (!ParseSingleElement(&aDef))
					return false;

				SexyString anId = (aXMLElement.mAttributes[_S("id")]);
				if(mProperties)
					mProperties->SetString(anId, (aDef));
			}
			else if (aXMLElement.mValue == _S("StringArray"))
			{
				StringVector aDef;

				if (!ParseStringArray(&aDef))
					return false;

				SexyString anId = (aXMLElement.mAttributes[_S("id")]);

				if(mProperties) 
					mProperties->SetStringVector(anId, aDef);
			}
			else if (aXMLElement.mValue == _S("Boolean"))
			{
				SexyString aVal;

				if (!ParseSingleElement(&aVal))
					return false;

				aVal = Upper(aVal);

				bool boolVal;
				if ((aVal == _S("1")) || (aVal == _S("YES")) || (aVal == _S("ON")) || (aVal == _S("TRUE")))
					boolVal = true;
				else if ((aVal == _S("0")) || (aVal == _S("NO")) || (aVal == _S("OFF")) || (aVal == _S("FALSE")))
					boolVal = false;
				else
				{
					Fail(_S("Invalid Boolean Value: '") + aVal + _S("'"));
					return false;
				}

				SexyString anId = (aXMLElement.mAttributes[_S("id")]);

				if(mProperties)
					mProperties->SetBoolean(anId, boolVal);
			}
			else if (aXMLElement.mValue == _S("Integer"))
			{
				SexyString aVal;

				if (!ParseSingleElement(&aVal))
					return false;

				int anInt;
				if (!StringToInt(aVal, &anInt))
				{
					Fail(_S("Invalid Integer Value: '") + aVal + _S("'"));
					return false;
				}

				SexyString anId = (aXMLElement.mAttributes[_S("id")]);

				if(mProperties)
					mProperties->SetInteger(anId, anInt);
			}
			else if (aXMLElement.mValue == _S("Double"))
			{
				SexyString aVal;

				if (!ParseSingleElement(&aVal))
					return false;

				double aDouble;
				if (!StringToDouble(aVal, &aDouble))
				{
					Fail(_S("Invalid Double Value: '") + aVal + _S("'"));
					return false;
				}

				SexyString anId = (aXMLElement.mAttributes[_S("id")]);

				if(mProperties)
					mProperties->SetDouble(anId, aDouble);
			}
			else
			{
				Fail(_S("Invalid Section '") + aXMLElement.mValue + _S("'"));
				return false;
			}
		}
		else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
		{
			Fail(_S("Element Not Expected '") + aXMLElement.mValue + _S("'"));
			return false;
		}		
		else if (aXMLElement.mType == XMLElement::TYPE_END)
		{
			return true;
		}
	}
}

/*!***********************************
// @return    	bool
//
// \brief		Internal Helper to parse XML
// \details 	<b>Description:</b> Internal Helper to parse XML
//************************************/
bool SexyPropertiesParser::DoParseProperties()
{
	if (!mXMLParser->HasFailed())
	{
		for (;;)
		{
			XMLElement aXMLElement;
			if (!mXMLParser->NextElement(&aXMLElement))
				break;

			if (aXMLElement.mType == XMLElement::TYPE_START)
			{
				if (aXMLElement.mValue == _S("Properties"))
				{
					if (!ParseProperties())
						break;
				}
				else 
				{
					Fail(_S("Invalid Section '") + aXMLElement.mValue + _S("'"));
					break;
				}
			}
			else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
			{
				Fail(_S("Element Not Expected '") + aXMLElement.mValue + _S("'"));
				break;
			}
		}
	}

	if (mXMLParser->HasFailed())
		Fail(mXMLParser->GetErrorText());	

	delete mXMLParser;
	mXMLParser = NULL;

	return !mHasFailed;
}

/*!***********************************
// @return    	bool
// @param     	theFilename
//
// \brief		Open the XML Properties File and load the values into the SexyProperties
// \details 	<b>Description:</b> Open the XML Properties File and load the values into the SexyProperties
//				Object passed in on the Ctor or SetProperties() method.
//************************************/
bool SexyPropertiesParser::ParsePropertiesFile(const SexyString& theFilename)
{
	if(Lower(GetFileExt(theFilename)) != Lower(_S("xml")))
		return ParsePropertiesFileBinary(theFilename);

	mXMLParser = new XMLParser();
	if(!mXMLParser->OpenFile(theFilename))
	{
		K_LOG("Sexy::SexyPropertiesParser::ParsePropertiesFile :Warning: Properties File not found: \"%s\".", SexyStringToStringFast(theFilename).c_str());
		return false;
	}

	if(mProperties == NULL)
		K_LOG("Sexy::SexyPropertiesParser::ParsePropertiesFile :Warning: Attempting to Parse Properties File \"%s\" with NULL Properties Pointer.", SexyStringToStringFast(theFilename).c_str());

	return DoParseProperties();	
}

/*!***********************************
// @return    	SexyString
//
// \brief		If the Parser Fails, this is the string text why
// \details 	<b>Description:</b> If the Parser Fails, this is the string text why
//************************************/
SexyString SexyPropertiesParser::GetErrorText()
{
	return mError;
}
/*!***********************************
// @return    	bool
// @param     	theFilename
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyPropertiesParser::ParsePropertiesFileBinary( const SexyString& theFilename )
{
	if(mProperties == NULL)
		K_LOG("Sexy::SexyPropertiesParser::ParsePropertiesFileBinary :Warning: Attempting to Parse Properties File \"%s\" with NULL Properties Pointer.", SexyStringToStringFast(theFilename).c_str());

	Sexy::Buffer aBuffer;

	if (Buffer::ReadBufferFromFile(theFilename, aBuffer))
	{
		unsigned char aReadMagicWord[4] = {0};
		unsigned char aMagicWord[4] = {0x73, 0x6b, 0x62,0x70};

		aBuffer.ReadBytes(aReadMagicWord, 4);

		for(int i = 0; i < 4; i++)
			if(aReadMagicWord[i] != aMagicWord[i])
			{
				K_LOG("Sexy::SexyPropertiesParser::ParsePropertiesFileBinary :Error: Failed to Parse Binary Properties File \"%s\" with Bad Header.", SexyStringToStringFast(theFilename).c_str());
				return false;
			}

		while(!aBuffer.AtEnd())
		{
			int aMetaTag = aBuffer.ReadLong();
			int aCount = 0;
			switch (aMetaTag)
			{
			case SexyProperties::INT_MAP_META_TAG:
				aCount = aBuffer.ReadLong();
				for (int i = 0; i < aCount && !aBuffer.AtEnd(); i++)
				{
					SexyString aKey = WStringToSexyStringFast(aBuffer.ReadUTF8String());
					int aValue = aBuffer.ReadLong();

					mProperties->SetInteger(aKey, aValue);
				}
				break;
			case SexyProperties::BOOL_MAP_META_TAG:
				aCount = aBuffer.ReadLong();
				for (int i = 0; i < aCount && !aBuffer.AtEnd(); i++)
				{
					SexyString aKey = WStringToSexyStringFast(aBuffer.ReadUTF8String());
					bool aValue = aBuffer.ReadBoolean();

					mProperties->SetBoolean(aKey, aValue);
				}
				break;
			case SexyProperties::DOUBLE_MAP_META_TAG:
				aCount = aBuffer.ReadLong();
				for (int i = 0; i < aCount && !aBuffer.AtEnd(); i++)
				{
					SexyString aKey = WStringToSexyStringFast(aBuffer.ReadUTF8String());
					double aValue = 0.0;
					aBuffer.ReadBytes((unsigned char *)&aValue, sizeof(double));
					mProperties->SetDouble(aKey, aValue);
				}
				break;
			case SexyProperties::STRING_MAP_META_TAG:
				aCount = aBuffer.ReadLong();
				for (int i = 0; i < aCount && !aBuffer.AtEnd(); i++)
				{
					SexyString aKey = WStringToSexyStringFast(aBuffer.ReadUTF8String());
					SexyString aValue = XMLDecodeString(WStringToSexyStringFast(aBuffer.ReadUTF8String()));

					mProperties->SetString(aKey, aValue);
				}
				break;
			case SexyProperties::STRING_VECTOR_MAP_META_TAG:
				aCount = aBuffer.ReadLong();
				for (int i = 0; i < aCount && !aBuffer.AtEnd(); i++)
				{
					SexyString aKey = WStringToSexyStringFast(aBuffer.ReadUTF8String());
					int aVectorSize = aBuffer.ReadLong();
					StringVector aVector;

					for (int j = 0; j < aVectorSize && !aBuffer.AtEnd(); j++)
					{
						aVector.push_back(WStringToSexyStringFast(aBuffer.ReadUTF8String()));
					}

					mProperties->SetStringVector(aKey, aVector);
				}
				break;
			}
		}

		return true;
	}
	return false;
}