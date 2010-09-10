// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "Common.h"
#include "MTRand.h"
#include "KSysDebug.h"
#include "KMiscTools.h"

#ifdef K_WIN32
#include <direct.h>
#include <io.h>
#include <aclapi.h>
#else
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "SexyConfig.h"
#include "Debug.h"

#include <sstream>
#include <stdio.h>

#ifndef min
#define min(a,b) (a < b) ? a : b
#endif

#ifndef max
#define max(a,b) (a > b) ? a : b
#endif

// Last include directive:
#include "memmgr.h"

static Sexy::MTRand gMTRand;
namespace Sexy
{
	SexyString gAppDataFolder = _S("");
    bool gDebug = false;
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Sexy::Rand()
{
	return gMTRand.Next();
}

/*!***********************************
// @return    	int
// @param     	range
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Sexy::Rand(int range)
{
	return gMTRand.Next((unsigned long)range);
}

/*!***********************************
// @return    	float
// @param     	range
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
float Sexy::Rand(float range)
{
	return gMTRand.Next(range);
}

/*!***********************************
// @return    	void
// @param     	theSeed
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::SRand(ulong theSeed)
{
	gMTRand.SRand(theSeed);
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::CheckFor98Mill()
{
	return false;
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::CheckForVista()
{
	return false;
}

#ifdef K_WIN32
typedef HRESULT (WINAPI *SHGetFolderPathFunc)(HWND, int, HANDLE, DWORD, LPTSTR);
void* GetSHGetFolderPath(const char* theDLL, HMODULE* theMod)
{
	HMODULE aMod = LoadLibraryA(theDLL);
	SHGetFolderPathFunc aFunc = NULL;

	if (aMod != NULL)
	{
		*((void**)&aFunc) = (void*)GetProcAddress(aMod, "SHGetFolderPathW");
		if (aFunc == NULL)
		{
			FreeLibrary(aMod);
			aMod = NULL;
		}
	}	

	*theMod = aMod;
	return aFunc;
}
#endif


/*!***********************************
// @return    	SexyString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyString Sexy::GetAppDataFolder()
{
	if (Sexy::gAppDataFolder == _S(""))
	{
#ifdef K_WIN32
		{
			HMODULE aMod;
			SHGetFolderPathFunc aFunc = (SHGetFolderPathFunc)GetSHGetFolderPath("shell32.dll", &aMod);
			if (aFunc == NULL || aMod == NULL)
				aFunc = (SHGetFolderPathFunc)GetSHGetFolderPath("shfolder.dll", &aMod);

			if (aMod != NULL)
			{
				TCHAR aPath[MAX_PATH];
				aFunc(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, (LPTSTR)aPath);

				SexyString aDataPath = StringToSexyStringFast(RemoveTrailingSlash(WStringToString(aPath))) 
					+ _S("\\") + (DEFAULT_COMPANY) + _S("\\") + (DEFAULT_TITLE) + _S("\\");
				Sexy::gAppDataFolder = aDataPath;
				MkDir(SexyStringToStringFast(aDataPath));
				AllowAllAccess(aDataPath);

				FreeLibrary(aMod);
			}
		}
#else
		Sexy::gAppDataFolder = StringToWString(KMiscTools::getPreferencesFolder())+ _S("/") + (DEFAULT_COMPANY) + _S("/") + (DEFAULT_TITLE) + _S("/");
#endif
	}

	return Sexy::gAppDataFolder;
}


/*!***********************************
// @return    	void
// @param     	thePath
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::SetAppDataFolder(const SexyString& thePath)
{
// 	if (CheckForVista())
	{
		SexyString aPath = thePath;
		if (!aPath.empty())
		{
			if (aPath[aPath.length()-1] != _S('\\') && aPath[aPath.length()-1] != _S('/'))
				aPath += _S('\\');
		}

		Sexy::gAppDataFolder = aPath;
	}
}

/*!***********************************
// @return    	std::string
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::URLEncode(const std::string& theString)
{
	char* aHexChars = "0123456789ABCDEF";

	std::string aString;

	for (unsigned i = 0; i < theString.length(); i++)
	{
		switch (theString[i])
		{
		case ' ':
			aString.insert(aString.end(), '+');
			break;
		case '?':
		case '&':
		case '%':
		case '+':
		case '\r':
		case '\n':
		case '\t':
			aString.insert(aString.end(), '%');
			aString.insert(aString.end(), aHexChars[(theString[i] >> 4) & 0xF]);
			aString.insert(aString.end(), aHexChars[(theString[i]     ) & 0xF]);
			break;
		default:
			aString.insert(aString.end(), theString[i]);
		}
	}

	return aString;
}

/*!***********************************
// @return    	std::string
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::StringToUpper(const std::string& theString)
{
	std::string aString;

	for (unsigned i = 0; i < theString.length(); i++)
		aString += toupper(theString[i]);

	return aString;
}

/*!***********************************
// @return    	std::wstring
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::StringToUpper(const std::wstring& theString)
{
	std::wstring aString;

	for (unsigned i = 0; i < theString.length(); i++)
		aString += towupper(theString[i]);

	return aString;
}

/*!***********************************
// @return    	std::string
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::StringToLower(const std::string& theString)
{
	std::string aString;

	for (unsigned i = 0; i < theString.length(); i++)
		aString += tolower(theString[i]);

	return aString;
}

/*!***********************************
// @return    	std::wstring
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::StringToLower(const std::wstring& theString)
{
	std::wstring aString;

	for (unsigned i = 0; i < theString.length(); ++i)
		aString += tolower(theString[i]);

	return aString;
}

/*!***********************************
// @return    	std::wstring
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::StringToWString(const std::string &theString)
{
	std::wstring aString;
	aString.reserve(theString.length());
	for(size_t i = 0; i < theString.length(); ++i)
		aString += (unsigned char)theString[i];
	return aString;
}

/*!***********************************
// @return    	std::string
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::WStringToString(const std::wstring &theString)
{
	size_t aRequiredLength = wcstombs( NULL, theString.c_str(), 0 );
	if (aRequiredLength < 16384)
	{
		char aBuffer[16384];
		wcstombs( aBuffer, theString.c_str(), 16384 );
		return std::string(aBuffer);
	}
	else
	{
		DBG_ASSERT(aRequiredLength != (size_t)-1);
		if (aRequiredLength == (size_t)-1) return "";

		char* aBuffer = new char[aRequiredLength+1];
		wcstombs( aBuffer, theString.c_str(), aRequiredLength+1 );
		std::string aStr = aBuffer;
		delete[] aBuffer;
		return aStr;
	}
}

/*!***********************************
// @return    	SexyString
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyString Sexy::StringToSexyString(const std::string& theString)
{
#ifdef _USE_WIDE_STRING
	return StringToWString(theString);
#else
	return SexyString(theString);
#endif
}

/*!***********************************
// @return    	SexyString
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyString Sexy::WStringToSexyString(const std::wstring &theString)
{
#ifdef _USE_WIDE_STRING
	return SexyString(theString);
#else
	return WStringToString(theString);
#endif
}

/*!***********************************
// @return    	std::string
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::SexyStringToString(const SexyString& theString)
{
#ifdef _USE_WIDE_STRING
	return WStringToString(theString);
#else
	return std::string(theString);
#endif
}

/*!***********************************
// @return    	std::wstring
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::SexyStringToWString(const SexyString& theString)
{
#ifdef _USE_WIDE_STRING
	return std::wstring(theString);
#else
	return StringToWString(theString);
#endif
}

/*!***********************************
// @return    	void
// @param     	theSourceString
// @param     	theTokens
// @param     	theDelimiter
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::Tokenize(const SexyString& theSourceString, std::vector<SexyString>& theTokens, const SexyString& theDelimiter)
{
	// If no delimiter is passed, tokenize on all white space.
	if (theDelimiter == _S(""))
	{
		SexyString aBuffer; // Have a buffer string
		SexyStringStream ss(theSourceString); // Insert the string into a stream

		while (ss >> aBuffer)
		{
			theTokens.push_back(aBuffer);
		}
	}

	// Skip delimiters at beginning.
	SexyString::size_type aLastPosition = theSourceString.find_first_not_of(theDelimiter, 0);

	// Find first "non-delimiter".
	SexyString::size_type aPosition = theSourceString.find_first_of(theDelimiter, aLastPosition);

	while (aPosition != SexyString::npos || aLastPosition != SexyString::npos)
	{
		// Found a token, add it to the vector.
		theTokens.push_back(theSourceString.substr(aLastPosition, aPosition - aLastPosition));

		// Skip delimiters.  Note the "not_of"
		aLastPosition = theSourceString.find_first_not_of(theDelimiter, aPosition);

		// Find next "non-delimiter"
		aPosition = theSourceString.find_first_of(theDelimiter, aLastPosition);
	}
}

/*!***********************************
// @return    	std::string
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::Trim(const std::string& theString)
{
	int aStartPos = 0;
	while ( aStartPos < (int) theString.length() && isspace((unsigned char)theString[aStartPos]) )
		aStartPos++;

	int anEndPos = theString.length() - 1;
	while ( anEndPos >= 0 && isspace((unsigned char)theString[anEndPos]) )
		anEndPos--;

	return theString.substr(aStartPos, anEndPos - aStartPos + 1);
}

/*!***********************************
// @return    	std::wstring
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::Trim(const std::wstring& theString)
{
	int aStartPos = 0;
	while ( aStartPos < (int) theString.length() && iswspace(theString[aStartPos]) )
		aStartPos++;

	int anEndPos = theString.length() - 1;
	while ( anEndPos >= 0 && iswspace(theString[anEndPos]) )
		anEndPos--;

	return theString.substr(aStartPos, anEndPos - aStartPos + 1);
}

/*!***********************************
// @return    	bool
// @param     	theString
// @param     	theIntVal
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::StringToInt(const std::string theString, int* theIntVal)
{
	*theIntVal = 0;

	if (theString.length() == 0)
		return false;

	int theRadix = 10;
	bool isNeg = false;

	unsigned i = 0;
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (; i < theString.length(); i++)
	{
		char aChar = theString[i];
		
		if ((theRadix == 10) && (aChar >= '0') && (aChar <= '9'))
			*theIntVal = (*theIntVal * 10) + (aChar - '0');
		else if ((theRadix == 0x10) && 
			(((aChar >= '0') && (aChar <= '9')) || 
			 ((aChar >= 'A') && (aChar <= 'F')) || 
			 ((aChar >= 'a') && (aChar <= 'f'))))
		{			
			if (aChar <= '9')
				*theIntVal = (*theIntVal * 0x10) + (aChar - '0');
			else if (aChar <= 'F')
				*theIntVal = (*theIntVal * 0x10) + (aChar - 'A') + 0x0A;
			else
				*theIntVal = (*theIntVal * 0x10) + (aChar - 'a') + 0x0A;
		}
		else if (((aChar == 'x') || (aChar == 'X')) && (i == 1) && (*theIntVal == 0))
		{
			theRadix = 0x10;
		}
		else
		{
			*theIntVal = 0;
			return false;
		}
	}

	if (isNeg)
		*theIntVal = -*theIntVal;

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theString
// @param     	theIntVal
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::StringToInt(const std::wstring theString, int* theIntVal)
{
	*theIntVal = 0;

	if (theString.length() == 0)
		return false;

	int theRadix = 10;
	bool isNeg = false;

	unsigned i = 0;
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (; i < theString.length(); i++)
	{
		wchar_t aChar = theString[i];
		
		if ((theRadix == 10) && (aChar >= L'0') && (aChar <= L'9'))
			*theIntVal = (*theIntVal * 10) + (aChar - L'0');
		else if ((theRadix == 0x10) && 
			(((aChar >= L'0') && (aChar <= L'9')) || 
			 ((aChar >= L'A') && (aChar <= L'F')) || 
			 ((aChar >= L'a') && (aChar <= L'f'))))
		{			
			if (aChar <= L'9')
				*theIntVal = (*theIntVal * 0x10) + (aChar - L'0');
			else if (aChar <= L'F')
				*theIntVal = (*theIntVal * 0x10) + (aChar - L'A') + 0x0A;
			else
				*theIntVal = (*theIntVal * 0x10) + (aChar - L'a') + 0x0A;
		}
		else if (((aChar == L'x') || (aChar == L'X')) && (i == 1) && (*theIntVal == 0))
		{
			theRadix = 0x10;
		}
		else
		{
			*theIntVal = 0;
			return false;
		}
	}

	if (isNeg)
		*theIntVal = -*theIntVal;

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theString
// @param     	theDoubleVal
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::StringToDouble(const std::string theString, double* theDoubleVal)
{
	*theDoubleVal = 0.0;

	if (theString.length() == 0)
		return false;

	bool isNeg = false;

	unsigned i = 0;
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (; i < theString.length(); i++)
	{
		char aChar = theString[i];

		if ((aChar >= '0') && (aChar <= '9'))
			*theDoubleVal = (*theDoubleVal * 10) + (aChar - '0');
		else if (aChar == '.')
		{
			i++;
			break;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}
	}

	double aMult = 0.1;
	for (; i < theString.length(); i++)
	{
		char aChar = theString[i];

		if ((aChar >= '0') && (aChar <= '9'))
		{
			*theDoubleVal += (aChar - '0') * aMult;	
			aMult /= 10.0;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}
	}

	if (isNeg)
		*theDoubleVal = -*theDoubleVal;

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theString
// @param     	theDoubleVal
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::StringToDouble(const std::wstring theString, double* theDoubleVal)
{
	*theDoubleVal = 0.0;

	if (theString.length() == 0)
		return false;

	bool isNeg = false;

	unsigned i = 0;
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (; i < theString.length(); i++)
	{
		wchar_t aChar = theString[i];

		if ((aChar >= L'0') && (aChar <= L'9'))
			*theDoubleVal = (*theDoubleVal * 10) + (aChar - L'0');
		else if (aChar == L'.')
		{
			i++;
			break;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}
	}

	double aMult = 0.1;
	for (; i < theString.length(); i++)
	{
		wchar_t aChar = theString[i];

		if ((aChar >= L'0') && (aChar <= L'9'))
		{
			*theDoubleVal += (aChar - L'0') * aMult;	
			aMult /= 10.0;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}
	}

	if (isNeg)
		*theDoubleVal = -*theDoubleVal;

	return true;
}

// TODO: Use <locale> for localization of number output?
/*!***********************************
// @return    	SexyString
// @param     	theValue
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyString Sexy::CommaSeperate(int theValue)
{	
	if (theValue == 0)
		return _S("0");

	SexyString aCurString;

	int aPlace = 0;
	int aCurValue = theValue;

	while (aCurValue > 0)
	{
		if ((aPlace != 0) && (aPlace % 3 == 0))
			aCurString = _S(',') + aCurString;
		aCurString = (SexyChar) (_S('0') + (aCurValue % 10)) + aCurString;
		aCurValue /= 10;
		aPlace++;
	}

	return aCurString;
}

#ifdef K_WIN32
#undef getcwd
#define getcwd _getcwd
#endif /* Def WIN32 or Def WIN64 */

/*!***********************************
// @return    	std::string
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::GetCurDir()
{
	char aDir[256];
	return getcwd(aDir, sizeof(aDir));
}

/*!***********************************
// @return    	std::string
// @param     	theRelPath
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::GetFullPath(const std::string& theRelPath)
{
	return GetPathFrom(theRelPath, GetCurDir());
}

/*!***********************************
// @return    	std::string
// @param     	theRelPath
// @param     	theDir
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::GetPathFrom(const std::string& theRelPath, const std::string& theDir)
{
	std::string aDriveString;
	std::string aNewPath = theDir;

	if ((theRelPath.length() >= 2) && (theRelPath[1] == ':'))
		return theRelPath;

	char aSlashChar = '/';

	if ((theRelPath.find('\\') != -1) || (theDir.find('\\') != -1))
		aSlashChar = '\\';	

	if ((aNewPath.length() >= 2) && (aNewPath[1] == ':'))
	{
		aDriveString = aNewPath.substr(0, 2);
		aNewPath.erase(aNewPath.begin(), aNewPath.begin()+2);
	}

	// Append a trailing slash if necessary
	if ((aNewPath.length() > 0) && (aNewPath[aNewPath.length()-1] != '\\') && (aNewPath[aNewPath.length()-1] != '/'))
		aNewPath += aSlashChar;

	std::string aTempRelPath = theRelPath;

	for (;;)
	{
		if (aNewPath.length() == 0)
			break;

		int aFirstSlash = aTempRelPath.find('\\');
		int aFirstForwardSlash = aTempRelPath.find('/');

		if ((aFirstSlash == -1) || ((aFirstForwardSlash != -1) && (aFirstForwardSlash < aFirstSlash)))
			aFirstSlash = aFirstForwardSlash;

		if (aFirstSlash == -1)
			break;

		std::string aChDir = aTempRelPath.substr(0, aFirstSlash);

		aTempRelPath.erase(aTempRelPath.begin(), aTempRelPath.begin() + aFirstSlash + 1);						

		if (aChDir.compare("..") == 0)
		{			
			int aLastDirStart = aNewPath.length() - 1;
			while ((aLastDirStart > 0) && (aNewPath[aLastDirStart-1] != '\\') && (aNewPath[aLastDirStart-1] != '/'))
				aLastDirStart--;

			std::string aLastDir = aNewPath.substr(aLastDirStart, aNewPath.length() - aLastDirStart - 1);
			if (aLastDir.compare("..") == 0)
			{
				aNewPath += "..";
				aNewPath += aSlashChar;
			}
			else
			{
				aNewPath.erase(aNewPath.begin() + aLastDirStart, aNewPath.end());
			}
		}		
		else if (aChDir.compare("") == 0)
		{
			aNewPath = aSlashChar;
			break;
		}
		else if (aChDir.compare(".") != 0)
		{
			aNewPath += aChDir + aSlashChar;
			break;
		}
	}

	aNewPath = aDriveString + aNewPath + aTempRelPath;

	if (aSlashChar == '/')
	{
		for (;;)
		{
			int aSlashPos = aNewPath.find('\\');
			if (aSlashPos == -1)
				break;
			aNewPath[aSlashPos] = '/';
		}
	}
	else
	{
		for (;;)
		{
			int aSlashPos = aNewPath.find('/');
			if (aSlashPos == -1)
				break;
			aNewPath[aSlashPos] = '\\';
		}
	}

	return aNewPath;
}

/*!***********************************
// @return    	bool
// @param     	theFileName
// @param     	isDir Is this a Directory?
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::AllowAllAccess(const SexyString& theFileName, bool isDir)
{
#if defined(WIN32) || defined(WIN64)
	HMODULE aLib = LoadLibraryA("advapi32.dll");
	if (aLib == NULL)
		return false;

	BOOL (WINAPI *fnSetFileSecurity)(LPCTSTR lpFileName, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor);
	BOOL (WINAPI *fnSetSecurityDescriptorDacl)(PSECURITY_DESCRIPTOR pSecurityDescriptor, BOOL bDaclPresent, PACL pDacl, BOOL bDaclDefaulted);
	BOOL (WINAPI *fnInitializeSecurityDescriptor)(PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD dwRevision);
	BOOL (WINAPI *fnAllocateAndInitializeSid)(
		PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority,
		BYTE nSubAuthorityCount,
		DWORD dwSubAuthority0,
		DWORD dwSubAuthority1,
		DWORD dwSubAuthority2,
		DWORD dwSubAuthority3,
		DWORD dwSubAuthority4,
		DWORD dwSubAuthority5,
		DWORD dwSubAuthority6,
		DWORD dwSubAuthority7,
		PSID* pSid
		);
	DWORD (WINAPI *fnSetEntriesInAcl)(ULONG cCountOfExplicitEntries, PEXPLICIT_ACCESS pListOfExplicitEntries, PACL OldAcl, PACL* NewAcl);
	PVOID (WINAPI *fnFreeSid)(PSID pSid);

	*(void**)&fnSetFileSecurity = (void*)GetProcAddress(aLib, "SetFileSecurityW");
	*(void**)&fnSetSecurityDescriptorDacl = (void*)GetProcAddress(aLib, "SetSecurityDescriptorDacl");
	*(void**)&fnInitializeSecurityDescriptor = (void*)GetProcAddress(aLib, "InitializeSecurityDescriptor");
	*(void**)&fnAllocateAndInitializeSid = (void*)GetProcAddress(aLib, "AllocateAndInitializeSid");
	*(void**)&fnSetEntriesInAcl = (void*)GetProcAddress(aLib, "SetEntriesInAclA");
	*(void**)&fnFreeSid = (void*) GetProcAddress(aLib, "FreeSid");

	if (!(fnSetFileSecurity && fnSetSecurityDescriptorDacl && fnInitializeSecurityDescriptor && fnAllocateAndInitializeSid && fnSetEntriesInAcl && fnFreeSid))
	{
		FreeLibrary(aLib);
		return false;
	}


	PSID pEveryoneSID = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	bool result = false;

	// Create a well-known SID for the Everyone group.
	if (fnAllocateAndInitializeSid(&SIDAuthWorld, 1,
		SECURITY_WORLD_RID,
		0, 0, 0, 0, 0, 0, 0,
		&pEveryoneSID))
	{
		EXPLICIT_ACCESS ea;

		// Initialize an EXPLICIT_ACCESS structure for an ACE.
		// The ACE will allow Everyone read access to the key.
		ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
		ea.grfAccessPermissions = STANDARD_RIGHTS_ALL | SPECIFIC_RIGHTS_ALL;
		ea.grfAccessMode = SET_ACCESS;
		ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
		ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
		ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
		ea.Trustee.ptstrName = (LPTSTR) pEveryoneSID;

		// Create a new ACL that contains the new ACEs.
		PACL pACL = NULL; 
		if (fnSetEntriesInAcl(1, &ea, NULL, &pACL) == ERROR_SUCCESS)
		{		
			// Initialize a security descriptor.  
			PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR) new char[SECURITY_DESCRIPTOR_MIN_LENGTH]; 

			if (fnInitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) 
			{  							 
				// Add the ACL to the security descriptor. 
				if (fnSetSecurityDescriptorDacl(pSD, 
					TRUE,     // bDaclPresent flag   
					pACL, 
					FALSE))   // not a default DACL 
				{
					if (fnSetFileSecurity(SexyStringToWStringFast(theFileName).c_str(), DACL_SECURITY_INFORMATION, pSD))
						result = true;
				}
			}

			delete [] pSD;
		}
	}

	FreeLibrary(aLib);
	return result;
#else
	if (chmod(SexyStringToStringFast(theFileName).c_str(), 
		S_IREAD|S_IWRITE|S_IRUSR|S_IWUSR|(isDir?S_IRWXU:0)|
		S_IRGRP|S_IWGRP|(isDir?S_IRWXG:0)|
		S_IROTH|S_IWOTH|(isDir?S_IRWXO:0)) == 0)
		return true;
	return false;
#endif
	return true;
}

bool processDelTreeFile(const char *lpszEntryName, bool bIsFolder, void *lpCallBackData)
{
	if (bIsFolder)
	{
		return Sexy::Deltree(lpszEntryName);
	}
	else
	{
		if(remove(lpszEntryName) != 0)
			return false;
	}
	return true;
}

/*!***********************************
// @return    	bool
// @param     	thePath
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::Deltree(const std::string& thePath)
{
	KMiscTools::enumerateFolder(thePath.c_str(), processDelTreeFile);

#ifdef K_WIN32
	return (_rmdir(thePath.c_str()) == 0);
#else
	return (rmdir(thePath.c_str()) == 0);
#endif
}

/*!***********************************
// @return    	bool
// @param     	theFileName
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::FileExists(const std::string& theFileName)
{
	return KMiscTools::fileExists((theFileName.c_str()));
}

bool Sexy::FileExists(const std::wstring& theFileName)
{
	return KMiscTools::fileExists(WStringToString(theFileName).c_str());
}

/*!***********************************
// @return    	void
// @param     	theDir
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::MkDir(const std::string& theDir)
{
	std::string aPath = theDir;

	int aCurPos = 0;
	for (;;)
	{
		int aSlashPos = aPath.find_first_of("\\/", aCurPos);
		if (aSlashPos == -1)
		{
			KMiscTools::createDirectory(aPath.c_str());
			break;
		}

		aCurPos = aSlashPos+1;

		std::string aCurPath = aPath.substr(0, aSlashPos);
		KMiscTools::createDirectory(aCurPath.c_str());
	}
}

/*!***********************************
// @return    	std::string
// @param     	thePath
// @param     	noExtension
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::GetFileName(const std::string& thePath, bool noExtension)
{
	int aLastSlash = max((int) thePath.rfind('\\'), (int) thePath.rfind('/'));

	if (noExtension)
	{
		int aLastDot = (int)thePath.rfind('.');
		if (aLastDot > aLastSlash)
			return thePath.substr(aLastSlash + 1, aLastDot - aLastSlash - 1);
	}

	if (aLastSlash == -1)
		return thePath;
	else
		return thePath.substr(aLastSlash + 1);
}

/*!***********************************
// @return    	std::wstring
// @param     	thePath
// @param     	noExtension
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::GetFileName( const std::wstring& thePath, bool noExtension /*= false*/ )
{
	int aLastSlash = max((int) thePath.rfind(L'\\'), (int) thePath.rfind(L'/'));

	if (noExtension)
	{
		int aLastDot = (int)thePath.rfind(L'.');
		if (aLastDot > aLastSlash)
			return thePath.substr(aLastSlash + 1, aLastDot - aLastSlash - 1);
	}

	if (aLastSlash == -1)
		return thePath;
	else
		return thePath.substr(aLastSlash + 1);
}
/*!***********************************
// @return    	std::string
// @param     	thePath
// @param     	withSlash
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::GetFileDir(const std::string& thePath, bool withSlash)
{
	int aLastSlash = max((int) thePath.rfind('\\'), (int) thePath.rfind('/'));

	if (aLastSlash == -1)
		return "";
	else
	{
		if (withSlash)
			return thePath.substr(0, aLastSlash+1);
		else
			return thePath.substr(0, aLastSlash);
	}
}

/*!***********************************
// @return    	std::wstring
// @param     	thePath
// @param     	withSlash
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::GetFileDir( const std::wstring& thePath, bool withSlash /*= false*/ )
{
	int aLastSlash = max((int) thePath.rfind(L'\\'), (int) thePath.rfind(L'/'));

	if (aLastSlash == -1)
		return L"";
	else
	{
		if (withSlash)
			return thePath.substr(0, aLastSlash+1);
		else
			return thePath.substr(0, aLastSlash);
	}

}
/*!***********************************
// @return    	std::string
// @param     	theDirectory
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::RemoveTrailingSlash(const std::string& theDirectory)
{
	int aLen = theDirectory.length();
	
	if ((aLen > 0) && ((theDirectory[aLen-1] == '\\') || (theDirectory[aLen-1] == '/')))
		return theDirectory.substr(0, aLen - 1);
	else
		return theDirectory;
}

/*!***********************************
// @return    	std::string
// @param     	theDirectory
// @param     	backSlash
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string	Sexy::AddTrailingSlash(const std::string& theDirectory, bool backSlash)
{
	if (!theDirectory.empty())
	{
		char aChar = theDirectory[theDirectory.length()-1];
		if (aChar!='\\' && aChar!='/')
			return theDirectory + (backSlash?'\\':'/');
		else
			return theDirectory;
	}
	else
		return "";
}


/*!***********************************
// @return    	time_t
// @param     	theFileName
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
time_t Sexy::GetFileDate(const std::string& theFileName)
{
	time_t aFileDate = 0;

	return aFileDate;
}

/*!***********************************
// @return    	std::string
// @param     	fmt
// @param     	argPtr
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::vformat(const char* fmt, va_list argPtr) 
{
    // We draw the line at a 1MB string.
    const int maxSize = 1000000;

    // If the string is less than 161 characters,
    // allocate it on the stack because this saves
    // the malloc/free time.
    const int bufSize = 161;
	char stackBuffer[bufSize];

    int attemptedSize = bufSize - 1;

	int numChars = 0;
#ifdef _WIN32
	numChars = _vsnprintf(stackBuffer, attemptedSize, fmt, argPtr);
#else
	numChars = vsnprintf(stackBuffer, attemptedSize, fmt, argPtr);
#endif

	//cout << "NumChars: " << numChars << endl;

    if ((numChars >= 0) && (numChars <= attemptedSize)) 
	{
		// Needed for case of 160-character printf thing
		stackBuffer[numChars] = '\0';

        // Got it on the first try.
		return std::string(stackBuffer);
    }

    // Now use the heap.
    char* heapBuffer = NULL;

    while (((numChars == -1) || (numChars > attemptedSize)) && 
		(attemptedSize < maxSize)) 
	{
        // Try a bigger size
        attemptedSize *= 2;
		heapBuffer = (char*)realloc(heapBuffer, (attemptedSize + 1));
#ifdef _WIN32
		numChars = _vsnprintf(heapBuffer, attemptedSize, fmt, argPtr);
#else
		numChars = vsnprintf(heapBuffer, attemptedSize, fmt, argPtr);
#endif
    }

	heapBuffer[numChars] = 0;

	std::string result = std::string(heapBuffer);

    free(heapBuffer);
    return result;
}

//overloaded StrFormat: should only be used by the xml strings
std::string Sexy::StrFormat(const char* fmt ...) 
{
    va_list argList;
    va_start(argList, fmt);
	std::string result = vformat(fmt, argList);
    va_end(argList);

    return result;
}

/*!***********************************
// @return    	std::wstring
// @param     	fmt
// @param     	argPtr
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::vformat(const wchar_t* fmt, va_list argPtr) 
{
    // We draw the line at a 1MB string.
    const int maxSize = 1000000;

    // If the string is less than 161 characters,
    // allocate it on the stack because this saves
    // the malloc/free time.
    const int bufSize = 161;
	wchar_t stackBuffer[bufSize];

    int attemptedSize = bufSize - 1;

	int numChars = 0;
#ifdef _WIN32
	numChars = _vsnwprintf(stackBuffer, attemptedSize, fmt, argPtr);
#else
	numChars = vswprintf(stackBuffer, attemptedSize, fmt, argPtr);
#endif

	//cout << "NumChars: " << numChars << endl;

    if ((numChars >= 0) && (numChars <= attemptedSize)) 
	{
		// Needed for case of 160-character printf thing
		stackBuffer[numChars] = '\0';

        // Got it on the first try.
		return std::wstring(stackBuffer);
    }

    // Now use the heap.
	wchar_t* heapBuffer = NULL;

    while (((numChars == -1) || (numChars > attemptedSize)) && 
		(attemptedSize < maxSize)) 
	{
        // Try a bigger size
        attemptedSize *= 2;
		heapBuffer = (wchar_t*)realloc(heapBuffer, (attemptedSize + 1)*sizeof(wchar_t));
#ifdef _WIN32
		numChars = _vsnwprintf(heapBuffer, attemptedSize, fmt, argPtr);
#else
		numChars = vswprintf(heapBuffer, attemptedSize, fmt, argPtr);
#endif
    }

	heapBuffer[numChars] = 0;

	std::wstring result(heapBuffer);

    free(heapBuffer);

    return result;
}

void findandreplace( std::wstring & source, const std::wstring & find, const std::wstring & replace )
{
	size_t j; 
	for (;(j = source.find( find )) != std::wstring::npos;) 
	{
		source.replace( j, find.length(), replace );

	}
}

//overloaded StrFormat: should only be used by the xml strings
/*!***********************************
// @return    	std::wstring
// @param     	...
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::StrFormat(const wchar_t* fmt ...)
{
#ifdef K_WIN32
	va_list argList;
	va_start(argList, fmt);
	std::wstring result = vformat(fmt, argList);
	va_end(argList);
#else	
	std::wstring aFmtStr = fmt;
	findandreplace(aFmtStr, L"%s", L"%ls");
	const wchar_t* fmt2 = aFmtStr.c_str();
	va_list argList;
	va_start(argList, fmt2);
	std::wstring result = vformat(fmt2, argList);
	va_end(argList);
#endif

    return result;
}

/*!***********************************
// @return    	std::string
// @param     	theString
// @param     	theDefinesMap
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::Evaluate(const std::string& theString, const DefinesMap& theDefinesMap)
{
	std::string anEvaluatedString = theString;

	for (;;)
	{
		int aPercentPos = anEvaluatedString.find('%');

		if (aPercentPos == std::string::npos)
			break;
		
		int aSecondPercentPos = anEvaluatedString.find('%', aPercentPos + 1);
		if (aSecondPercentPos == std::string::npos)
			break;

		std::string aName = anEvaluatedString.substr(aPercentPos + 1, aSecondPercentPos - aPercentPos - 1);

		std::string aValue;
		DefinesMap::const_iterator anItr = theDefinesMap.find(aName);		
		if (anItr != theDefinesMap.end())
			aValue = anItr->second;
		else
			aValue = "";		

		anEvaluatedString.erase(anEvaluatedString.begin() + aPercentPos, anEvaluatedString.begin() + aSecondPercentPos + 1);
		anEvaluatedString.insert(anEvaluatedString.begin() + aPercentPos, aValue.begin(), aValue.begin() + aValue.length());
	}

	return anEvaluatedString;
}

/*!***********************************
// @return    	std::string
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::XMLDecodeString(const std::string& theString)
{
	std::string aNewString;

	for (ulong i = 0; i < theString.length(); i++)
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
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::XMLDecodeString(const std::wstring& theString)
{
	std::wstring aNewString;

	for (ulong i = 0; i < theString.length(); i++)
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
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::XMLEncodeString(const std::string& theString)
{
	std::string aNewString;

	bool hasSpace = false;

	for (ulong i = 0; i < theString.length(); i++)
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
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::XMLEncodeString(const std::wstring& theString)
{
	std::wstring aNewString;

	bool hasSpace = false;

	for (ulong i = 0; i < theString.length(); i++)
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

/*!***********************************
// @return    	std::string
// @param     	_data
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::Upper(const std::string& _data)
{
	std::string s = _data;
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	return s;
}

/*!***********************************
// @return    	std::wstring
// @param     	_data
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::Upper(const std::wstring& _data)
{
	std::wstring s = _data;
	std::transform(s.begin(), s.end(), s.begin(), towupper);
	return s;
}

/*!***********************************
// @return    	std::string
// @param     	_data
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::Lower(const std::string& _data)
{
	std::string s = _data;
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}

/*!***********************************
// @return    	std::wstring
// @param     	_data
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::Lower(const std::wstring& _data)
{
	std::wstring s = _data;
	std::transform(s.begin(), s.end(), s.begin(), towlower);
	return s;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	int
// @param     	theStr
// @param     	theFind
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Sexy::StrFindNoCase(const char *theStr, const char *theFind)
{
	int p1,p2;
	int cp = 0;
	const int len1 = (int)strlen(theStr);
	const int len2 = (int)strlen(theFind);
	while(cp < len1)
	{
		p1 = cp;
		p2 = 0;
		while(p1<len1 && p2<len2)
		{
			if(tolower(theStr[p1])!=tolower(theFind[p2]))
				break;

			p1++; p2++;
		}
		if(p2==len2)
			return p1-len2;

		cp++;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	bool
// @param     	theStr
// @param     	thePrefix
// @param     	maxLength
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::StrPrefixNoCase(const char *theStr, const char *thePrefix, int maxLength)
{
	int i;
	char c1 = 0, c2 = 0;
	for (i=0; i<maxLength; i++)
	{
		c1 = tolower(*theStr++);
		c2 = tolower(*thePrefix++);

		if (c1==0 || c2==0)
			break;

		if (c1!=c2)
			return false;
	}

	return c2==0 || i==maxLength;
}


/*!***********************************
// @return    	std::string
// @param     	thePath
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Sexy::GetFileExt( const std::string& thePath )
{
	std::string aFileName = GetFileName(thePath);
	std::string aFileNameSansExt = GetFileName(thePath, true);

	if (aFileName.size() > aFileNameSansExt.size())
	{
		return aFileName.substr(aFileNameSansExt.size()+1);
	}
	return "";
}

/*!***********************************
// @return    	std::wstring
// @param     	thePath
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Sexy::GetFileExt( const std::wstring& thePath )
{
	std::string aFileName = GetFileName(WStringToString(thePath));
	std::string aFileNameSansExt = GetFileName(WStringToString(thePath), true);

	if (aFileName.size() > aFileNameSansExt.size())
	{
		return StringToWString(aFileName.substr(aFileNameSansExt.size()+1));
	}
	return L"";
}

#ifndef K_IPHONE
/*!***********************************
// @return    	void
// @param     	theString
// @param     	theWindow
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::CopyToClipboard(const SexyString& theString, K_WNDHANDLE theWindow)
{
#ifdef K_WIN32

	HGLOBAL aGlobalHandle;
	char* theData;	
	WCHAR* theWData;

	std::string aString = SexyStringToStringFast(theString);

	if (OpenClipboard(theWindow))
	{
		EmptyClipboard();

		aGlobalHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, theString.length()+1);
		theData = (char*) GlobalLock(aGlobalHandle);
		strcpy(theData, aString.c_str());
		GlobalUnlock(aGlobalHandle);
		SetClipboardData(CF_TEXT, aGlobalHandle);
		SetClipboardData(CF_OEMTEXT, aGlobalHandle);		
		SetClipboardData(CF_LOCALE, aGlobalHandle);

		int aSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, aString.c_str(), aString.length(), NULL, 0);
		aGlobalHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (aSize + 1) * sizeof(WCHAR));
		theWData = (WCHAR*) GlobalLock(aGlobalHandle);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, aString.c_str(), aString.length(), theWData, aSize);
		theWData[aSize] = '\0';
		GlobalUnlock(aGlobalHandle);
		SetClipboardData(CF_UNICODETEXT, aGlobalHandle);
		
		CloseClipboard();
	}
#else
	
	/* UNTESTED - but something like this should work. */
	/*
	OSStatus err = noErr;
	PasteboardRef theClipboard;

    err = PasteboardCreate(kPasteboardClipboard, &theClipboard);
	// check err
	err = PasteboardClear(theClipboard);
	// check err
	CFDataRef textData = CFDataCreate(kCFAllocatorDefault, (UInt8*)theString.c_str(), theString.length() * sizeof(wchar_t);
	err = PasteboardPutItemFlavor(inPasteboard, (PasteboardItemID)1, CFSTR("public.utf16-plain-text"), textData, 0);
	// check err
									  //*/
#endif
}

/*!***********************************
// @return    	std::wstring
// @param     	theWindow
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyString Sexy::GetClipboard(K_WNDHANDLE theWindow)
{
#ifdef K_WIN32

	HGLOBAL aGlobalHandle;	
	std::string aString;

	if (OpenClipboard(theWindow))
	{
		aGlobalHandle = GetClipboardData(CF_TEXT);	
		if (aGlobalHandle != NULL)
		{
			char* theData = (char*) GlobalLock(aGlobalHandle);
			if (theData != NULL)
			{
				aString = theData;		
				GlobalUnlock(aGlobalHandle);
			}
		}
		
		CloseClipboard();
	}

	return StringToSexyStringFast(aString);

#else
	/* UNTESTED - but something like this should work. */
	/*
	OSStatus err = noErr;
	ItemCount itemCount;
	PasteboardRef theClipboard;

    err = PasteboardCreate(kPasteboardClipboard, &theClipboard);
	// check err
	err = PasteboardGetItemCount(theClipboard, &itemCount);
	// check err
	for (UInt32 itemIndex = 1; itemIndex <= itemCount; itemIndex++)
	{
		PasteboardItemID itemID;
		CFArrayRef flavorTypeArray;
		CFIndex flavorCount;

		err = PasteboardGetItemIdentifier(theClipboard, itemIndex, &itemID);
		// check err
		err = PasteboardCopyItemFlavors(theClipboard, itemID, &flavorTypeArray);
		// check err
		flavorCount = CFArrayGetCount(flavorTypeArray);
		for (CFIndex flavorIndex = 0; flavorIndex < flavorCount; flavorIndex++)
		{
			CFStringRef flavorType;
			CFDataRef flavorData;
			CFIndex flavorDataSize;
			char flavorText[256];

			flavorType = (CFStringRef)CFArrayGetValueAtIndex(flavorTypeArray, flavorIndex);
			if (UTTypeConformsTo(flavorType, CFSTR("public.utf16-plain-text")))
			{
				err = PasteboardCopyItemFlavorData(inPasteboard, itemID, flavorType, &flavorData);
				// check err
				flavorDataSize = CFDataGetLength(flavorData);
				flavorDataSize = (flavorDataSize<254) ? flavorDataSize : 254;

				for (short dataIndex = 0; dataIndex <= flavorDataSize; dataIndex++)
				{
					char byte = *(CFDataGetBytePtr( flavorData ) + dataIndex);
					flavorText[dataIndex] = byte;
				}

				flavorText[flavorDataSize] = '\0';
				CFRelease(flavorData);

				// Convert and return flavorText?
				// Break out of loop when anything found?
			}
		}

		CFRelease(flavorTypeArray);
	}
	//*/
#endif
	return _S("");
}
#endif // ifndef K_IPHONE

/*!***********************************
// @return    	SexyString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyString Sexy::GetSystemFontsFolder()
{
	static SexyString aSysFontFolder = _S("");

	if(aSysFontFolder == _S(""))
	{
#ifdef K_WIN32
		{
			HMODULE aMod;
			SHGetFolderPathFunc aFunc = (SHGetFolderPathFunc)GetSHGetFolderPath("shell32.dll", &aMod);
			if (aFunc == NULL || aMod == NULL)
				aFunc = (SHGetFolderPathFunc)GetSHGetFolderPath("shfolder.dll", &aMod);

			if (aMod != NULL)
			{
				TCHAR aPath[MAX_PATH];
				aFunc(NULL, CSIDL_WINDOWS, NULL, SHGFP_TYPE_CURRENT, (LPTSTR)aPath);

				std::wstring aStrPath = aPath;
				if(aPath != L"")
					aSysFontFolder = StringToSexyStringFast(RemoveTrailingSlash(WStringToString(aStrPath))) + _S("\\Fonts\\");

				FreeLibrary(aMod);
			}
		}
#else
		aSysFontFolder = _S("/Library/Fonts/");
#endif
	}

	return aSysFontFolder;
}

/*!***********************************
// @return    	bool
// @param     	theFileName
//
// \brief		Delete a File.
// \details 	<b>Description:</b> Delete a File.
//************************************/
bool Sexy::RemoveFile( const std::string& theFileName )
{
	return (remove(theFileName.c_str()) == 0);
}

/*!***********************************
// @return    	bool
// @param     	theFileName
//
// \brief		Delete a File.
// \details 	<b>Description:</b> Delete a File.
//************************************/
bool Sexy::RemoveFile( const std::wstring& theFileName )
{
	return RemoveFile(WStringToString(theFileName));
}