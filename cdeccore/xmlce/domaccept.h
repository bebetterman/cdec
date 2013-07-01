#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class _XmlDomAccepter: public ISaxAccepter
{
	DECLARE_REF_CLASS(_XmlDomAccepter)

protected:
	ref<XmlDocument>				m_pDoc;
	ref<ArrayList<XmlElement> >		m_stkElements;

public:
	_XmlDomAccepter(ref<XmlDocument> pDoc): m_pDoc(pDoc)
	{
		if (pDoc == NULL)
			cdec_throw(Exception(EC_InvalidArg));
		m_stkElements = gc_new<ArrayList<XmlElement> >();
	}

public:
	void BeginDocument();
	void EndDocument();
	void BeginElement(stringx name);
	void EndElement(stringx name);
	void AddAttribute(stringx name, stringx value);
	void AddValue(stringx value);
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
