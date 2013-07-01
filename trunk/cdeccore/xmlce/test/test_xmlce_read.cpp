// -------------------------------------------------------------------------- //
//	文件名		：	test_xmlce_read.cpp
//	创建者		：	杨钢
//	创建时间	：	4/29/2008 9:15:49 AM
//	功能描述	：	
//
// -------------------------------------------------------------------------- //

#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestXmlceReader: public UnitTestSuite
{
	UNITTEST_SUITE(TestXmlceReader)
		UNITTEST_METHOD(testParseBasic)
		UNITTEST_METHOD(testParseFileAnsi)
		UNITTEST_METHOD(testParseFileUnicode)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testParseBasic()
	{
		stringx xmlText = __X("<document />");

		ref<XmlDocument> doc = gc_new<XmlDocument>();
		doc->LoadXml(xmlText);

		ref<XmlElement> eRoot = doc->get_DocumentElement();
		UNITTEST_ASSERT(eRoot->get_NodeType() == XMLNODE_ELEMENT);
		UNITTEST_ASSERT(eRoot->get_NodeName() == __X("document"));
		UNITTEST_ASSERT(eRoot->get_SubNodes()->Empty());
		UNITTEST_ASSERT(eRoot->get_Attributes()->Empty());
	}

	void testParseFileAnsi()
	{
#ifdef X_OS_WINDOWS
		// Windows-1252
		stringx sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_encode1252.xml"));
		const WCHAR _cs1[] = { 'Z', 0xD6, 0 };
		stringx cs1 = _cs1;
		verifyMessage(sampleFile, cs1);

		// GB2312
		sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_encode936.xml"));
		verifyMessage(sampleFile, __X("Z\x4E2D"));	// Z中

		// Default CP, No <?xml ... ?> element
		sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_encode_acp.xml"));
		verifyMessage(sampleFile, __X("Hello"));

		// Default CP, <?xml ... ?> element without encoding attribute
		sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_encode_acp2.xml"));
		verifyMessage(sampleFile, __X("Hello"));
#else
#endif
	}


	void testParseFileUnicode()
	{
		// UTF-8
		stringx sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_encode65001.xml"));
		const WCHAR _cs1[] = { 'Z', 0xD6, 0x4E2D, 0 };	// 中
		stringx cs1 = _cs1;
		verifyMessage(sampleFile, cs1);

		// UTF-8 without decoding exclaiming
		sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_encode65001ns.xml"));
		verifyMessage(sampleFile, cs1);

		// UTF-16
		sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_encode1200.xml"));
		verifyMessage(sampleFile, cs1);

		// UTF-16 without decoding exclaiming
		sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_encode1200ns.xml"));
		verifyMessage(sampleFile, cs1);

		// UTF-16 with decoding exclaimed "unicode" instead of "utf-16"
		sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_encode1200us.xml"));
		verifyMessage(sampleFile, cs1);
	}

	void tearDown()
	{
	}

private:
	void verifyMessage(stringx filename, stringx pwszCheck)
	{
		ref<XmlDocument> doc = gc_new<XmlDocument>();
		doc->Load(filename);
		ref<XmlElement> e = doc->get_DocumentElement();
		ref<XmlAttribute> attr = e->get_Attribute(__X("message"));
		UNITTEST_ASSERT(attr->get_Value() == pwszCheck);
	}
};

UNITTEST_SUITE_REGISTRATION(TestXmlceReader);

// -------------------------------------------------------------------------- //
#endif
