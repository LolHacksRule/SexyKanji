// SexyKanjiTestSuite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MiniCppUnit.hxx"
#include "KPTK.h"

#ifdef K_WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif // WIN32

int _tmain(int argc, _TCHAR* argv[])
{
	/* Read data files from application bundle (for Mac) */
	KMiscTools::setFileSource(true);
#ifdef K_WIN32
	_chdir( KMiscTools::makeFilePath( "" ) );
#else
	chdir( KMiscTools::makeFilePath( "" ) );
#endif

	return TestFixtureFactory::theInstance().runTests() ? 0 : -1;
}

