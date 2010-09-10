#pragma once
#include "MiniCppUnit.hxx"
#include "XMLParser.h"
#include "XMLWriter.h"

class XMLTestFixture : public TestFixture<XMLTestFixture>
{
protected:
	Sexy::XMLParser		mParser;
public:
	TEST_FIXTURE( XMLTestFixture )
	{
 		TEST_CASE( testSetStringSourceW );
 		TEST_CASE( testSetStringSourceA );
 		TEST_CASE( testParseStringSourceW );
 		TEST_CASE( testParseStringSourceA );

		TEST_CASE( testOpenXML_ANSI);
		TEST_CASE( testOpenXML_UTF8);
		TEST_CASE( testOpenXML_UCS2_LE);
		TEST_CASE( testOpenXML_UCS2_LE);

		TEST_CASE( testParseXML_ANSI);
		TEST_CASE( testParseXML_UTF8);
		TEST_CASE( testParseXML_UCS2_LE);
		TEST_CASE( testParseXML_UCS2_LE);

// 		TEST_CASE( testAssertMessage );
// 		TEST_CASE( testAdditionInts );
// 		TEST_CASE( testDoubles );
// 		TEST_CASE( testFloats );
// 		TEST_CASE( testLongDoubles );
// 		TEST_CASE( testNotANumber );
// 		TEST_CASE( testComparisonWithEpsilon );
// 		TEST_CASE( testException );
// 		TEST_CASE( testStringAddition );
	}

public:
	virtual void setUp();
	virtual void tearDown();

	void	testSetStringSourceW();
	void	testSetStringSourceA();
	void	testParseStringSourceW();
	void	testParseStringSourceA();

	void	testOpenXML_ANSI();
	void	testOpenXML_UTF8();
	void	testOpenXML_UCS2_LE();
	void	testOpenXML_UCS2_BE();

	void	testParseXML_ANSI();

	void	parseToEnd();
	void	testParseXML_UTF8();
	void	testParseXML_UCS2_LE();
	void	testParseXML_UCS2_BE();
};

REGISTER_FIXTURE( XMLTestFixture );