// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "SexyPropertiesWriter.h"
#include "XMLWriter.h"
#include "Buffer.h"

#include "SexyProperties.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

#define MAX_SEXYPROP_WRITER_BUFFER_SIZE 64

/*!***********************************
// @return    	
// @param     	theProperties
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyPropertiesWriter::SexyPropertiesWriter(SexyProperties* theProperties)
{
	mProperties = theProperties;
	mHasFailed = false;
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyPropertiesWriter::~SexyPropertiesWriter()
{
	mProperties = NULL;
}
/*!***********************************
// @return    	bool
// @param     	theFilename
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyPropertiesWriter::WritePropertiesFile( const SexyString& theFilename )
{
	if (mProperties == NULL) return Fail(_S("Error: Cannot write NULL Properties Pointer."));

	if(Lower(GetFileExt(theFilename)) != Lower(_S("xml"))) 
		return WritePropertiesFileBinary(theFilename);
	
	XMLWriter aWriter;

	if(aWriter.OpenFile(theFilename) && !aWriter.HasFailed())
	{
		aWriter.StartElement(_S("Properties"));
		{
			StringIntMap::iterator int_map_itr = mProperties->mIntProperties.begin();
			for (; int_map_itr != mProperties->mIntProperties.end(); int_map_itr++)
			{
				aWriter.StartElement(_S("Integer"));
				aWriter.WriteAttribute(_S("id"), int_map_itr->first);
				aWriter.WriteElementText(StrFormat(_S("%d"), int_map_itr->second));
			}

			StringBoolMap::iterator bool_map_itr = mProperties->mBoolProperties.begin();
			for (; bool_map_itr != mProperties->mBoolProperties.end(); bool_map_itr++)
			{
				aWriter.StartElement(_S("Boolean"));
				aWriter.WriteAttribute(_S("id"), bool_map_itr->first);
				aWriter.WriteElementText(bool_map_itr->second ? _S("true") : _S("false"));
			}

			StringDoubleMap::iterator double_map_itr = mProperties->mDoubleProperties.begin();
			for (; double_map_itr != mProperties->mDoubleProperties.end(); double_map_itr++)
			{
				aWriter.StartElement(_S("Double"));
				aWriter.WriteAttribute(_S("id"), double_map_itr->first);
				aWriter.WriteElementText(StrFormat(_S("%f"), double_map_itr->second));
			}

			StringStringMap::iterator str_map_itr = mProperties->mStringProperties.begin();
			for (; str_map_itr != mProperties->mStringProperties.end(); str_map_itr++)
			{
				aWriter.StartElement(_S("String"));
				aWriter.WriteAttribute(_S("id"), str_map_itr->first);
				aWriter.WriteElementText(XMLEncodeString(str_map_itr->second));
			}

			StringStringVectorMap::iterator str_vec_map_itr = mProperties->mStringVectorProperties.begin();
			for (; str_vec_map_itr != mProperties->mStringVectorProperties.end(); str_vec_map_itr++)
			{
				aWriter.StartElement(_S("StringVector"));
				aWriter.WriteAttribute(_S("id"), str_vec_map_itr->first);
				//aWriter.WriteElementText(StrFormat(_S("%f"), str_vec_map_itr->second));
				for (unsigned int i = 0; i < str_vec_map_itr->second.size(); i++)
				{
					aWriter.StartElement(_S("String"));
					aWriter.WriteElementText(XMLEncodeString(str_vec_map_itr->second[i]));
				}
				aWriter.StopElement(/*S("StringVector")*/);
			}
		}
		aWriter.StopElement(/*("Properties")*/);

		return aWriter.CloseFile();;
	}

	return false;
}
/*!***********************************
// @return    	bool
// @param     	theErrorText
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyPropertiesWriter::Fail( const SexyString& theErrorText )
{
	if (!mHasFailed)
	{
		mHasFailed = true;
		mError = theErrorText;
		K_LOG("Sexy::SexyPropertiesWriter::Fail :Error: %s", SexyStringToStringFast(mError).c_str());
	}

	return false;
}
/*!***********************************
// @return    	bool
// @param     	theFilename
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyPropertiesWriter::WritePropertiesFileBinary( const SexyString& theFilename )
{
	if(mProperties == NULL) return Fail(_S("Error: Cannot write NULL Properties Pointer."));

	unsigned char MagicWord[4] = {0x73, 0x6b, 0x62,0x70};

	Sexy::Buffer aBuffer;
	aBuffer.WriteBytes(MagicWord, 4);

	aBuffer.WriteLong(SexyProperties::INT_MAP_META_TAG);
	aBuffer.WriteLong(mProperties->mIntProperties.size());
	StringIntMap::iterator int_map_itr = mProperties->mIntProperties.begin();
	for (; int_map_itr != mProperties->mIntProperties.end(); int_map_itr++)
	{
		aBuffer.WriteUTF8String(SexyStringToWStringFast(int_map_itr->first));
		aBuffer.WriteLong(int_map_itr->second);
	}

	aBuffer.WriteLong(SexyProperties::BOOL_MAP_META_TAG);
	aBuffer.WriteLong(mProperties->mBoolProperties.size());
	StringBoolMap::iterator bool_map_itr = mProperties->mBoolProperties.begin();
	for (; bool_map_itr != mProperties->mBoolProperties.end(); bool_map_itr++)
	{
		aBuffer.WriteUTF8String(SexyStringToWStringFast(bool_map_itr->first));
		aBuffer.WriteBoolean(bool_map_itr->second);
	}

	aBuffer.WriteLong(SexyProperties::DOUBLE_MAP_META_TAG);
	aBuffer.WriteLong(mProperties->mDoubleProperties.size());
	StringDoubleMap::iterator double_map_itr = mProperties->mDoubleProperties.begin();
	for (; double_map_itr != mProperties->mDoubleProperties.end(); double_map_itr++)
	{
		aBuffer.WriteUTF8String(SexyStringToWStringFast(double_map_itr->first));
		aBuffer.WriteBytes(((unsigned char*)(&double_map_itr->second)), sizeof(double));
	}

	aBuffer.WriteLong(SexyProperties::STRING_MAP_META_TAG);
	aBuffer.WriteLong(mProperties->mStringProperties.size());
	StringStringMap::iterator str_map_itr = mProperties->mStringProperties.begin();
	for (; str_map_itr != mProperties->mStringProperties.end(); str_map_itr++)
	{
		aBuffer.WriteUTF8String(SexyStringToWStringFast(str_map_itr->first));
		aBuffer.WriteUTF8String(XMLEncodeString(SexyStringToWStringFast(str_map_itr->second)));
	}

	aBuffer.WriteLong(SexyProperties::STRING_VECTOR_MAP_META_TAG);
	aBuffer.WriteLong(mProperties->mStringVectorProperties.size());
	StringStringVectorMap::iterator str_vec_map_itr = mProperties->mStringVectorProperties.begin();
	for (; str_vec_map_itr != mProperties->mStringVectorProperties.end(); str_vec_map_itr++)
	{
		aBuffer.WriteUTF8String(SexyStringToWStringFast(str_vec_map_itr->first));
		aBuffer.WriteLong(str_vec_map_itr->second.size());
		for (unsigned int i = 0; i < str_vec_map_itr->second.size(); i++)
		{
			aBuffer.WriteUTF8String(XMLEncodeString(SexyStringToWStringFast(str_vec_map_itr->second[i])));
		}
	}

	return Sexy::Buffer::WriteBufferToFile(theFilename, aBuffer);
}
