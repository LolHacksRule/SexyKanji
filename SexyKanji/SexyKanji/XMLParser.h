// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__

#include <map>
#include <vector>
#include <list>

#include "Common.h"

namespace Sexy
{

	class XMLParam
	{
	public:
		SexyString				mKey;
		SexyString				mValue;
	};

	typedef std::map<SexyString, SexyString>	XMLParamMap;
	typedef std::list<XMLParamMap::iterator>	XMLParamMapIteratorList;

	typedef std::vector<wchar_t> XMLParserBuffer;

	class XMLElement
	{
	public:
		enum
		{
			TYPE_NONE,
			TYPE_START,
			TYPE_END,
			TYPE_ELEMENT,
			TYPE_INSTRUCTION,
			TYPE_COMMENT
		};
	public:

		int						mType;
		SexyString				mSection;
		SexyString				mValue;
		SexyString				mInstruction;
		XMLParamMap				mAttributes;
		XMLParamMapIteratorList	mAttributeIteratorList; // stores attribute iterators in their original order
	};

	class XMLParser
	{
	protected:
		std::string				mFileName;
		SexyString				mErrorText;
		int						mLineNum;
		bool					mHasFailed;
		bool					mAllowComments;
		XMLParserBuffer			mBufferedText;
		SexyString				mSection;
		bool					mForcedEncodingType;
		bool					mFirstChar;
		bool					mByteSwap;

	public:
		enum XMLEncodingType
		{
			ASCII,
			UTF_8,
			UTF_16,
			UTF_16_LE,
			UTF_16_BE
		};

	public:
		XMLParser();
		virtual ~XMLParser();

		//////////////////////////////////////////////////////////////////////////
		// XML Parser Interface
		//////////////////////////////////////////////////////////////////////////
	public:
		bool					OpenFile(const std::wstring& theFilename);
		bool					OpenFile(const std::string& theFilename);
		void					SetStringSource(const std::wstring& theString);
		void					SetStringSource(const std::string& theString);

		bool					NextElement(XMLElement* theElement);

		SexyString				GetErrorText();
		int						GetCurrentLineNum();
		std::string				GetFileName();

		inline void				AllowComments(bool doAllow) { mAllowComments = doAllow; }

		bool					HasFailed();
		bool					EndOfFile();

		//////////////////////////////////////////////////////////////////////////
		// XML Utility Functions
		//////////////////////////////////////////////////////////////////////////
		static std::string		XMLDecodeString(const std::string& theString);
		static std::string		XMLEncodeString(const std::string& theString);
		static std::wstring		XMLDecodeString(const std::wstring& theString);
		static std::wstring		XMLEncodeString(const std::wstring& theString);

		static bool				HasAttribute(XMLElement* theNode, SexyString theAttrbiute);
		

	protected:
		bool					Fail(const SexyString& theErrorText);
		void					Init();

		bool					AddAttribute(XMLElement* theElement, const SexyString& aAttributeKey, const SexyString& aAttributeValue);
	};

};

#endif //__XMLPARSER_H__
