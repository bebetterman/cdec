// -------------------------------------------------------------------------- //
//	文件名		：	test_xmlce_dom.cpp
//	创建者		：	杨钢
//	创建时间	：	5/7/2008 5:06:13 PM
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

const WCHAR ChsText1[] = { 0x68A6, 0x60F3, 0x91D1, 0x5C71, 0 };	// "梦想金山"

class TestXmlceDom: public UnitTestSuite
{
	UNITTEST_SUITE(TestXmlceDom)
		UNITTEST_METHOD(testBasicModel)
		UNITTEST_METHOD(testReadChildNodes)
		UNITTEST_METHOD(testReadAttributes)
		UNITTEST_METHOD(testReadNodeValue)
	UNITTEST_SUITE_END();

public:
	void setUp()
	{
	}

	void testBasicModel()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_library.xml"));
		ref<XmlDocument>	doc = gc_new<XmlDocument>();
		doc->Load(sampleFile);
		ref<XmlElement> eLibrary = doc->get_DocumentElement();
		UNITTEST_ASSERT(eLibrary->get_NodeType() == XMLNODE_ELEMENT && eLibrary->get_NodeName() == __X("library"));
		XmlAttributeCollection attrs = eLibrary->get_Attributes();
		UNITTEST_ASSERT(attrs->Count() == 1);
		ref<XmlAttribute> attr = attrs->at(0);
		UNITTEST_ASSERT(attr->get_Name() == __X("name") && attr->get_Value() == __X("Demo Library"));

		XmlNodeCollection subNodes = eLibrary->get_SubNodes();
		UNITTEST_ASSERT(subNodes->Count() == 2);
		ref<XmlNode> nodeOwner = subNodes->at(0);
		UNITTEST_ASSERT(nodeOwner->get_NodeType() == XMLNODE_ELEMENT && nodeOwner->get_NodeName() == __X("owner"));
		ref<XmlNode> nodeBooks = subNodes->at(1);
		UNITTEST_ASSERT(nodeBooks->get_NodeType() == XMLNODE_ELEMENT && nodeBooks->get_NodeName() == __X("books"));

		ref<XmlElement> eOwner = ref_cast<XmlElement>(nodeOwner);
		XmlNodeCollection subNodesOwner = eOwner->get_SubNodes();
		UNITTEST_ASSERT(subNodesOwner->Count() == 1);
		ref<XmlNode> nodeValue = subNodesOwner->at(0);
		UNITTEST_ASSERT(nodeValue->get_NodeType() == XMLNODE_VALUE && nodeValue->get_NodeName() == __X("#text"));
		ref<XmlNodeValue> vOwner = ref_cast<XmlNodeValue>(nodeValue);
		UNITTEST_ASSERT(vOwner->get_Value() == __X("Somebody"));
	}

	void testReadChildNodes()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_library.xml"));
		ref<XmlDocument>	doc = gc_new<XmlDocument>();
		doc->Load(sampleFile);
		ref<XmlElement> eLibrary = doc->get_DocumentElement();

		ref<XmlElement> eBooks = eLibrary->SelectSingleNode(__X("books"));
		UNITTEST_ASSERT(eBooks != NULL && eBooks->get_NodeName() == __X("books"));
		UNITTEST_ASSERT(eLibrary->SelectSingleNode(__X("Owner")) == NULL);

		ref<ArrayList<XmlElement> > arrayBook = gc_new<ArrayList<XmlElement> >();
		//arrayBook->resize(8, NULL);
		ref<XmlElement>	temp = NULL;
		for (size_t i = 0; i < 8; ++i)
		{
			arrayBook->Add(temp);
		}
		eBooks->SelectNodes(__X("book"), arrayBook);
		UNITTEST_ASSERT(arrayBook->Count() == 2);
		UNITTEST_ASSERT(arrayBook->at(0)->get_NodeName() == __X("book"));
		UNITTEST_ASSERT(arrayBook->at(0)->get_Attribute(__X("title"))->get_Value() == ChsText1);
		UNITTEST_ASSERT(arrayBook->at(1)->get_NodeName() == __X("book"));
		UNITTEST_ASSERT(arrayBook->at(1)->get_Attribute(__X("title"))->get_Value() == __X("My Diary"));
		UNITTEST_ASSERT(eBooks->SelectSingleNode(__X("book")) == arrayBook->at(0));

		ref<XmlElement> eVirtual = eBooks->SelectSingleNode(__X("virtual"));
		UNITTEST_ASSERT(eVirtual != NULL && eVirtual->get_NodeName() == __X("virtual"));
		UNITTEST_ASSERT(eVirtual->get_SubNodes()->Count() == 0);

		XmlNodeCollection subNodesBooks = eBooks->get_SubNodes();
		ref<ArrayList<XmlElement> >	arraySubNodes = gc_new<ArrayList<XmlElement> >();
		for (size_t i = 0; i < subNodesBooks->Count(); ++i)
		{
			ref<XmlNode>	nodeBook = subNodesBooks->at(i);
			UNITTEST_ASSERT(nodeBook->get_NodeType() == XMLNODE_ELEMENT);
			ref<XmlElement>	eBook = ref_cast<XmlElement>(nodeBook);
			arraySubNodes->Add(eBook);
		}
		UNITTEST_ASSERT(arraySubNodes->Count() == 3);
		UNITTEST_ASSERT(arraySubNodes->at(0) == arrayBook->at(0));
		UNITTEST_ASSERT(arraySubNodes->at(1) == eVirtual);
		UNITTEST_ASSERT(arraySubNodes->at(2) == arrayBook->at(1));
	}

	void testReadAttributes()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_library.xml"));
		ref<XmlDocument>	doc = gc_new<XmlDocument>();
		doc->Load(sampleFile);
		ref<XmlElement> eLibrary = doc->get_DocumentElement();
		ref<XmlElement> eBooks = eLibrary->SelectSingleNode(__X("books"));		

		ref<XmlElement> eBook0 = eBooks->SelectSingleNode(__X("book"));
		ref<XmlAttribute> attr = eBook0->get_Attribute(__X("price"));
		UNITTEST_ASSERT(attr != NULL && attr->get_Name() == __X("price") && attr->get_Value() == __X("42"));
		UNITTEST_ASSERT(eBook0->get_Attribute(__X("Price")) == 0);

		XmlAttributeCollection attrsBook = eBook0->get_Attributes();
		ref<ArrayList<XmlAttribute> >	arrayAttr = gc_new<ArrayList<XmlAttribute> >();
		for (size_t i = 0; i < attrsBook->Count(); ++i)
			arrayAttr->Add(attrsBook->at(i));
		UNITTEST_ASSERT(arrayAttr->Count() == 2);
		UNITTEST_ASSERT(arrayAttr->at(0)->get_Name() == __X("title") && arrayAttr->at(0)->get_Value() == ChsText1);
		UNITTEST_ASSERT(arrayAttr->at(1)->get_Name() == __X("price") && arrayAttr->at(1)->get_Value() == __X("42"));

		ref<XmlElement> eBook0Comment = eBook0->SelectSingleNode(__X("comments"));
		UNITTEST_ASSERT(eBook0Comment->get_Attributes()->Count() == 0);
		UNITTEST_ASSERT(eBook0Comment->get_Attribute(__X("value")) == NULL);
	}

	void testReadNodeValue()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("xmlr_samp_library.xml"));
		ref<XmlDocument>	doc = gc_new<XmlDocument>();
		doc->Load(sampleFile);
		ref<XmlElement> eLibrary = doc->get_DocumentElement();
		ref<XmlElement> eOwner = eLibrary->SelectSingleNode(__X("owner"));		
		UNITTEST_ASSERT(eOwner->get_ValueText() == __X("Somebody"));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestXmlceDom);

// -------------------------------------------------------------------------- //
#endif
