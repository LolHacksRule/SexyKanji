#include "XMLTestFixture.h"

using namespace Sexy;

std::wstring gTestFile_1_w = L"<!-- below is an example of an instruction. The XMLParser class can handle instruction syntax,\
but you'll notice that in the XMLDemo we don't actually use the value, we just ignore it. -->\
<?xml version=\"1.0\"?>\
\
<!-- Sections contain nested items -->\
<Section1>\
\
<!-- This item, \"Item1\", contains just 1 attribute, \"text\", and nothing more -->\
<Item1 text=\"Some text for Section1/Item1\"/>\
\
<!-- This item, \"BoolParam\", contains an attribute, \"text\", as well as an element value,\
which is the string \"true\" -->\
<BoolParam text=\"Some text for Section1/BoolParam\">true</BoolParam>\
\
<!-- end of the section -->\
</Section1>\
\
<!-- This section contains an attribute in its line -->\
<Section2 s2attrib=\"Section2 attrib text\">\
<IntParam text=\"Some text for Section2/IntParam\">1234</IntParam>\
\
<!-- This just shows that you can split things up across lines. This is functionally equivalent to:\
<MultiAttrib attrib1=\"attrib1\" attrib2=\"2\" attrib3=\"woo! attrib3!\"/>	-->\
\
<MultiAttrib \
attrib1=\"attrib1\"\
attrib2=\"2\"\
attrib3=\"woo! attrib3!\"\
/>\
\
<!-- The XMLParser class handles nested sections as well -->\
<Subsection>\
<!-- and of course, you can put more items in here, like any other section -->\
<Item2>Text for Section2/Subsection/Item2</Item2>\
</Subsection>\
\
</Section2>";

std::string gTestFile_1_a = "<!-- below is an example of an instruction. The XMLParser class can handle instruction syntax,\
but you'll notice that in the XMLDemo we don't actually use the value, we just ignore it. -->\
<?xml version=\"1.0\"?>\
\
<!-- Sections contain nested items -->\
<Section1>\
\
<!-- This item, \"Item1\", contains just 1 attribute, \"text\", and nothing more -->\
<Item1 text=\"Some text for Section1/Item1\"/>\
\
<!-- This item, \"BoolParam\", contains an attribute, \"text\", as well as an element value,\
which is the string \"true\" -->\
<BoolParam text=\"Some text for Section1/BoolParam\">true</BoolParam>\
\
<!-- end of the section -->\
</Section1>\
\
<!-- This section contains an attribute in its line -->\
<Section2 s2attrib=\"Section2 attrib text\">\
<IntParam text=\"Some text for Section2/IntParam\">1234</IntParam>\
\
<!-- This just shows that you can split things up across lines. This is functionally equivalent to:\
<MultiAttrib attrib1=\"attrib1\" attrib2=\"2\" attrib3=\"woo! attrib3!\"/>	-->\
\
<MultiAttrib \
attrib1=\"attrib1\"\
attrib2=\"2\"\
attrib3=\"woo! attrib3!\"\
/>\
\
<!-- The XMLParser class handles nested sections as well -->\
<Subsection>\
<!-- and of course, you can put more items in here, like any other section -->\
<Item2>Text for Section2/Subsection/Item2</Item2>\
</Subsection>\
\
</Section2>";

void XMLTestFixture::setUp()
{
}

void XMLTestFixture::tearDown()
{
}

void XMLTestFixture::testSetStringSourceW()
{
	mParser.SetStringSource(gTestFile_1_w);
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());

	mParser.SetStringSource(L"");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());	
}

void XMLTestFixture::testSetStringSourceA()
{
	mParser.SetStringSource(gTestFile_1_a);
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());

	mParser.SetStringSource("");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());
}

void XMLTestFixture::testParseStringSourceW()
{
	mParser.SetStringSource(gTestFile_1_w);
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());

	parseToEnd();
}

void XMLTestFixture::testParseStringSourceA()
{
	mParser.SetStringSource(gTestFile_1_a);
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());

	parseToEnd();
}

void XMLTestFixture::testOpenXML_ANSI()
{
	mParser.OpenFile("xml/demo_ansi.xml");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());
}

void XMLTestFixture::testOpenXML_UTF8()
{
	mParser.OpenFile("xml/demo_utf8.xml");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());
}

void XMLTestFixture::testOpenXML_UCS2_LE()
{
	mParser.OpenFile("xml/demo_ucs2_le.xml");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());
}

void XMLTestFixture::testOpenXML_UCS2_BE()
{
	mParser.OpenFile("xml/demo_ucs2_be.xml");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());
}

void XMLTestFixture::testParseXML_ANSI()
{
	mParser.OpenFile("xml/demo_ansi.xml");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());

	parseToEnd();
}

void XMLTestFixture::testParseXML_UTF8()
{
	mParser.OpenFile("xml/demo_utf8.xml");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());

	parseToEnd();
}

void XMLTestFixture::testParseXML_UCS2_LE()
{
	mParser.OpenFile("xml/demo_ucs2_le.xml");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());

	parseToEnd();
}

void XMLTestFixture::testParseXML_UCS2_BE()
{
	mParser.OpenFile("xml/demo_ucs2_be.xml");
	ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());

	parseToEnd();
}

void XMLTestFixture::parseToEnd()
{
	try
	{
		XMLElement aNode;
		while(mParser.NextElement(&aNode))
		{
			ASSERT_MESSAGE(!mParser.HasFailed(), SexyStringToStringFast(mParser.GetErrorText()).c_str());
		}
	}
	catch (...)
	{
		FAIL("Parsing throws Exception.");
	}
}