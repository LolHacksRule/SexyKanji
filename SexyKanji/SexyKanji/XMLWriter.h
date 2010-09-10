// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
// XMLWriter.h: interface for the XMLWriter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLWRITER_H__3DB6D22D_A120_4738_B622_06E90FAED4E4__INCLUDED_)
#define AFX_XMLWRITER_H__3DB6D22D_A120_4738_B622_06E90FAED4E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#pragma warning(disable:4503)


#include <stack>
#include "XMLParser.h"
#include "Common.h"
#include "Buffer.h"

#define MAX_XML_WRITER_BUFFER_SIZE 256

namespace Sexy
{
	class XMLWriter  
	{
	private:
		long					mBufferWriteIndex;
		unsigned char			mWriteBuffer[MAX_XML_WRITER_BUFFER_SIZE];
		Buffer					mFileBuffer;
	protected:
		SexyString				mFileName;
		SexyString				mErrorText;
		int						mLineNum;
		bool					mHasFailed;
		bool					mAllowComments;
		bool					mOpenAttributes;
		bool					mFileClosed;

		std::stack<SexyString>	mSectionStack;
		std::stack<SexyString> mWarningStack;
		
	protected:
		bool					CheckFileOpen();
		bool					ValidateElementNodeName(const SexyString& theNodeName);
		void					Fail(const SexyString& theErrorText);
		void					Warn(const SexyString& theWarning);
		void					Init();
		void					Flush();
		void					CheckWriteBuffer();
		void					WriteStringToBuffer(std::string theString);
		void					WriteStringToBuffer(std::wstring theString);
		
	public:
		XMLWriter();
		virtual ~XMLWriter();
		
static	bool					AddAttribute(XMLElement* theElement, const SexyString& aAttributeKey, const SexyString& aAttributeValue);
		bool					WriteAttribute(const SexyString& aAttributeKey, const SexyString& aAttributeValue);
		bool					WriteAttribute(const SexyString& aAttributeKey, const float& aAttributeValue);
		bool					WriteAttribute(const SexyString& aAttributeKey, const int& aAttributeValue);
		bool					WriteAttribute(const SexyString& aAttributeKey);
		void					Comment(const SexyString& theComment);
		bool					StartElement(const SexyString &theElementName);
		bool					StartElement(XMLElement *theElement);
		bool					StopElement();
		bool					WriteElementText(SexyString theText);
		bool					OpenFile(const SexyString& theFilename);
		bool					CloseFile();
		SexyString				GetErrorText();
		int						GetCurrentLineNum();
		SexyString				GetFileName();
		
		inline void				AllowComments(bool doAllow) { mAllowComments = doAllow; }
		
		bool					HasFailed();
	};
};


#endif // !defined(AFX_XMLWRITER_H__3DB6D22D_A120_4738_B622_06E90FAED4E4__INCLUDED_)
