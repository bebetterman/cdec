// -------------------------------------------------------------------------- //
//	文件名		：	report_sax.h
//	创建者		：	杨钢
//	创建时间	：	2006-3-3 15:22:59
//	功能描述	：
//
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT XmlWriter: public ISaxAccepter
{
	DECLARE_REF_CLASS(XmlWriter)

private:
	ref<TextWriter> m_tw;
	int		m_nLevel;	// 缩进级别
	int		m_TagType;	// 0 <tag /> 1 <tag>text</tag> 2 <tag><subtag /><tag />

public:
	XmlWriter(ref<TextWriter> tw): m_tw(tw), m_nLevel(0), m_TagType(2) { }
	
	void BeginDocument()
	{
		BeginDocument(true, NULL);
	}

	void BeginDocument(bool fVersion, stringx pathXlst)
	{
		if (fVersion)
			m_tw->WriteLine(__X("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));

		if (pathXlst != NULL)
		{
			m_tw->Write(__X("<?xml-stylesheet type='text/xsl' href='"));
			m_tw->Write(pathXlst);
			m_tw->WriteLine(__X("'?>"));
		}
	}

	void EndDocument() { m_tw->Close(); }

	void BeginElement(stringx name)
	{
		if (m_TagType == 0)
			m_tw->WriteLine(__X(">"));
		else if (m_TagType != 2)
			m_tw->WriteLine();

		WriteIndent();
		m_tw->WriteChar('<');
		m_tw->Write(name);

		++m_nLevel;
		m_TagType = 0;
	}

	void EndElement(stringx name)
	{
		--m_nLevel;
		switch (m_TagType)
		{
		case 0:
			m_tw->WriteLine(__X(" />"));
			break;
		case 2:
			WriteIndent();
			// 这里没有 break
		case 1:
			m_tw->Write(__X("</"));
			m_tw->Write(name);
			m_tw->WriteLine(__X(">"));
			break;
		}

		ASSERT(m_nLevel >= 0);
		m_TagType = 2;
	}

	void AddAttribute(stringx name, stringx value)
	{
		ASSERT(m_TagType == 0);
		m_tw->WriteChar(' ');
		m_tw->Write(name);
		m_tw->WriteChar('=');
		m_tw->WriteChar('\"');
		m_tw->Write(value);
		m_tw->WriteChar('\"');
	}

	void AddValue(stringx value)
	{
		ASSERT(m_nLevel != 1);
		switch (m_TagType)
		{
		case 0:
			m_tw->WriteChar('>');
			m_tw->Write(value);
			m_TagType = 1;
			break;
		case 2:
			WriteIndent();
			m_tw->WriteLine(value);
			break;
		}
	}

protected:
	void WriteIndent()
	{
		std::wstring16	wsTab;
		for (int i = 0; i < m_nLevel; ++i)
			wsTab.append(__X("  "));
		m_tw->Write((stringx)wsTab);
	}
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
