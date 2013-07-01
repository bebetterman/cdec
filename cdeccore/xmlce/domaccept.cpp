#include "stdafx.h"
#include "domaccept.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

void _XmlDomAccepter::BeginDocument()
{
	if (m_pDoc->get_DocumentElement() != NULL)
		cdec_throw(XmlException(EC_Unexpected));

	ASSERT(m_stkElements->Empty());
}

void _XmlDomAccepter::EndDocument()
{
	if (m_pDoc->get_DocumentElement() == NULL || !m_stkElements->Empty())
		cdec_throw(XmlException(EC_Unexpected));
}

void _XmlDomAccepter::BeginElement(stringx name)
{
	ref<XmlElement>	pElement = gc_new<XmlElement>(name);
	if (m_stkElements->Empty())
	{
		if (m_pDoc->get_DocumentElement() != NULL)
			cdec_throw(XmlException(EC_Unexpected));

		m_pDoc->set_Root(pElement);
	}
	else
	{
		ref<XmlElement>	pParent = m_stkElements->Last();
		pParent->AddChild(pElement);
	}
	m_stkElements->Add(pElement);
}

void _XmlDomAccepter::EndElement(stringx name)
{
	if (m_stkElements->Empty())
		cdec_throw(XmlException(EC_Unexpected));

	ref<XmlElement>	pParent = m_stkElements->Last();
	if (pParent->get_NodeName() != name)
		cdec_throw(XmlException(EC_Unexpected));

	m_stkElements->RemoveLast();
}

void _XmlDomAccepter::AddValue(stringx value)
{
	if (m_stkElements->Empty())
		cdec_throw(XmlException(EC_Unexpected));

	ref<XmlElement>	pParent = m_stkElements->Last();
	ref<XmlNodeValue>	pValue = gc_new<XmlNodeValue>(value);
	pParent->AddChild(pValue);
}

void _XmlDomAccepter::AddAttribute(stringx name, stringx value)
{
	if (m_stkElements->Empty())
		cdec_throw(XmlException(EC_Unexpected));

	ref<XmlElement>	pParent = m_stkElements->Last();
	ref<XmlAttribute>	pAttribute = gc_new<XmlAttribute>(name, value);
	pParent->AddAttribute(pAttribute);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
