// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __SEXYKANJI_COMMON_H__
#define __SEXYKANJI_COMMON_H__

#include <string>
#include <string.h>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <cstdlib>

#include "KSysDefs.h"

#ifdef __GNUC__
#define DEPRECATED(func, comment) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func, comment) __declspec(deprecated(comment)) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func, com) func
#endif


#ifdef K_WIN32
#define strcasecmp _stricmp
#define wcscasecmp _wcsicmp
#else
#define wcscasecmp wcscmp
#endif /* Def WIN32 or Def WIN64 */

// We are now using WideStrings by Default.  This is important for porting
// games to other languages.  Ever NOT port a game because Sexy didn't support
// it?  Now Sexy Fonts support WStrings out of the Box.
#define _USE_WIDE_STRING

#ifdef _USE_WIDE_STRING
	
	#define SexyStrEncoding		K_WCHAR
	typedef std::wstring		SexyString;
	typedef std::wstringstream	SexyStringStream;
	#define _S(x)				L ##x

	#define sexystrncmp			wcsncmp
	#define sexystrcmp			wcscmp
	#define sexystricmp			wcscmp
	#define sexysscanf			swscanf
#ifndef K_WIN32
	#define sexyatoi(a)			wcstol(a,0,10)
#else
	#define sexyatoi			_wtoi
#endif
	#define sexystrcpy			wcscpy

	#define SexyStringToStringFast(x)	WStringToString(x)
	#define SexyStringToWStringFast(x)	(x)
	#define StringToSexyStringFast(x)	StringToWString(x)
	#define WStringToSexyStringFast(x)	(x)
	
	struct StringLessNoCase { bool operator()(const std::wstring &s1, const std::wstring &s2) const { return wcscasecmp((s1).c_str(),(s2).c_str())<0; } };

#else 
	// use K_ANSI here ??? /// (lol, yeah ;-)
	#define SexyStrEncoding		K_ANSI
	typedef std::string			SexyString;
	typedef std::stringstream	SexyStringStream;
	#define _S(x)				x

	#define sexystrncmp			strncmp
	#define sexystrcmp			strcmp
	#define sexystricmp			_stricmp
	#define sexysscanf			sscanf
	#define sexyatoi			atoi
	#define sexystrcpy			strcpy

	#define SexyStringToStringFast(x)	(x)
	#define SexyStringToWStringFast(x)	StringToWString(x)
	#define StringToSexyStringFast(x)	(x)
	#define WStringToSexyStringFast(x)	WStringToString(x)
	
	struct StringLessNoCase { bool operator()(const std::string &s1, const std::string &s2) const { return strcasecmp(s1.c_str(),s2.c_str())<0; } };

#endif

#define SEXY_LOG(x)				K_LOG(SexyStringToStringFast(x).c_str())

#define LONG_BIGE_TO_NATIVE(l) (((l >> 24) & 0xFF) | ((l >> 8) & 0xFF00) | ((l << 8) & 0xFF0000) | ((l << 24) & 0xFF000000))
#define WORD_BIGE_TO_NATIVE(w) (((w >> 8) & 0xFF) | ((w << 8) & 0xFF00))
#define LONG_LITTLEE_TO_NATIVE(l) (l)
#define WORD_LITTLEE_TO_NATIVE(w) (w)

#define LENGTH(anyarray) (sizeof(anyarray) / sizeof(anyarray[0]))

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef signed long long int64;

typedef std::map<std::string, std::string>		DefinesMap;
typedef SexyString::value_type					SexyChar;

typedef std::vector<SexyString>					StringVector;
typedef std::map<SexyString, SexyString>		SexyStringSexyStringMap;
typedef std::map<SexyString, SexyString>		StringStringMap;
typedef std::map<std::string, std::wstring>		StringWStringMap;
typedef std::map<SexyString, bool>				StringBoolMap;
typedef std::map<SexyString, int>				StringIntMap;
typedef std::map<SexyString, double>			StringDoubleMap;
typedef std::map<SexyString, StringVector>		StringStringVectorMap;
typedef std::set<SexyString>					SexyStringSet;

#define HAS_SEXYCHAR

namespace Sexy
{

	const ulong SEXY_RAND_MAX = 0x7FFFFFFF;

	int					Rand();
	int					Rand(int range);
	float				Rand(float range);
	void				SRand(ulong theSeed);
	extern std::string	vformat(const char* fmt, va_list argPtr);
	extern std::wstring	vformat(const wchar_t* fmt, va_list argPtr);
	extern std::string	StrFormat(const char* fmt ...);
	extern std::wstring	StrFormat(const wchar_t* fmt ...);
	bool				CheckFor98Mill();
	bool				CheckForVista();
	SexyString			GetAppDataFolder();
	SexyString			GetSystemFontsFolder();
	void				SetAppDataFolder(const SexyString& thePath);
	std::string			URLEncode(const std::string& theString);
	std::string			StringToUpper(const std::string& theString);
	std::wstring		StringToUpper(const std::wstring& theString);
	std::string			StringToLower(const std::string& theString);
	std::wstring		StringToLower(const std::wstring& theString);
	std::wstring		StringToWString(const std::string &theString);
	std::string			WStringToString(const std::wstring &theString);
	SexyString			StringToSexyString(const std::string& theString);
	SexyString			WStringToSexyString(const std::wstring& theString);
	std::string			SexyStringToString(const SexyString& theString);
	std::wstring		SexyStringToWString(const SexyString& theString);
	void				Tokenize(const SexyString& theSourceString, std::vector<SexyString>& theTokens, const SexyString& theDelimiter = _S(""));
	std::string			Upper(const std::string& theData);
	std::wstring		Upper(const std::wstring& theData);
	std::string			Lower(const std::string& theData);
	std::wstring		Lower(const std::wstring& theData);
	std::string			Trim(const std::string& theString);
	std::wstring		Trim(const std::wstring& theString);
	bool				StringToInt(const std::string theString, int* theIntVal);
	bool				StringToDouble(const std::string theString, double* theDoubleVal);
	bool				StringToInt(const std::wstring theString, int* theIntVal);
	bool				StringToDouble(const std::wstring theString, double* theDoubleVal);
	int					StrFindNoCase(const char *theStr, const char *theFind);
	bool				StrPrefixNoCase(const char *theStr, const char *thePrefix, int maxLength = 10000000);
	SexyString			CommaSeperate(int theValue);
	std::string			Evaluate(const std::string& theString, const DefinesMap& theDefinesMap);
	std::string			XMLDecodeString(const std::string& theString);
	std::string			XMLEncodeString(const std::string& theString);
	std::wstring		XMLDecodeString(const std::wstring& theString);
	std::wstring		XMLEncodeString(const std::wstring& theString);

	bool				Deltree(const std::string& thePath);
	bool				FileExists(const std::string& theFileName);
	bool				FileExists(const std::wstring& theFileName);
	bool				RemoveFile(const std::string& theFileName);
	bool				RemoveFile(const std::wstring& theFileName);
	void				MkDir(const std::string& theDir);
	std::string			GetFileName(const std::string& thePath, bool noExtension = false);
	std::wstring		GetFileName(const std::wstring& thePath, bool noExtension = false);
	std::string			GetFileExt(const std::string& thePath);
	std::wstring		GetFileExt(const std::wstring& thePath);
	std::string			GetFileDir(const std::string& thePath, bool withSlash = false);
	std::wstring		GetFileDir(const std::wstring& thePath, bool withSlash = false);
	std::string			RemoveTrailingSlash(const std::string& theDirectory);
	std::string			AddTrailingSlash(const std::string& theDirectory, bool backSlash = false);
	time_t				GetFileDate(const std::string& theFileName);
	std::string			GetCurDir();
	std::string			GetFullPath(const std::string& theRelPath);
	std::string			GetPathFrom(const std::string& theRelPath, const std::string& theDir);
	bool				AllowAllAccess(const SexyString& theFileName, bool isDir = false);
	
#ifndef K_IPHONE
	void				CopyToClipboard(const SexyString& theString, K_WNDHANDLE theWindow);
	SexyString			GetClipboard(K_WNDHANDLE theWindow);
#endif

	inline void			inlineUpper(std::string &theData)
	{
		//std::transform(theData.begin(), theData.end(), theData.begin(), toupper);

		int aStrLen = (int) theData.length();
		for (int i = 0; i < aStrLen; i++)
		{
			theData[i] = toupper(theData[i]);
		}
	}

	inline void			inlineUpper(std::wstring &theData)
	{
		//std::transform(theData.begin(), theData.end(), theData.begin(), toupper);

		int aStrLen = (int) theData.length();
		for (int i = 0; i < aStrLen; i++)
		{
			theData[i] = towupper(theData[i]);
		}
	}

	inline void			inlineLower(std::string &theData)
	{
		std::transform(theData.begin(), theData.end(), theData.begin(), tolower);
	}

	inline void			inlineLower(std::wstring &theData)
	{
		std::transform(theData.begin(), theData.end(), theData.begin(), tolower);
	}

	inline void			inlineLTrim(std::string &theData, const std::string& theChars = " \t\r\n")
	{
		theData.erase(0, theData.find_first_not_of(theChars));
	}

	inline void			inlineLTrim(std::wstring &theData, const std::wstring& theChars = L" \t\r\n")
	{
		theData.erase(0, theData.find_first_not_of(theChars));
	}


	inline void			inlineRTrim(std::string &theData, const std::string& theChars = " \t\r\n")
	{
		theData.resize(theData.find_last_not_of(theChars) + 1);
	}

	inline void			inlineTrim(std::string &theData, const std::string& theChars = " \t\r\n")
	{
		inlineRTrim(theData, theChars);
		inlineLTrim(theData, theChars);
	}
/*
	inline std::wstring operator +(const  std::wstring& a, const std::string& b)
	{
		return a + Sexy::StringToWString(b);
	}

	inline std::string operator +(const  std::string& a, const std::wstring& b)
	{
		return a + Sexy::WStringToString(b);
	}

	inline std::wstring&  operator +=(std::wstring& a, const std::string& b)
	{
		return a += Sexy::StringToWString(b);
	}

	inline std::string&  operator +=(std::string& a, const std::wstring& b)
	{
		return a += Sexy::WStringToString(b);
	}

	inline bool  operator ==(const std::wstring& a, const std::string& b)
	{
		return (a == Sexy::StringToWString(b));
	}

	inline bool  operator ==(const std::string& a, const std::wstring& b)
	{
		return (a == Sexy::WStringToString(b));
	}

	inline bool  operator !=(const std::wstring& a, const std::string& b)
	{
		return (a != Sexy::StringToWString(b));
	}

	inline bool  operator !=(const std::string& a, const std::wstring& b)
	{
		return (a != Sexy::WStringToString(b));
	}//*/

} // namespace Sexy

#endif //__SEXYKANJI_COMMON_H__
