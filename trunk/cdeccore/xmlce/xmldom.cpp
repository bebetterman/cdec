// -------------------------------------------------------------------------- //
//	文件名		：	xmldom.inl
//	创建者		：	杨钢
//	创建时间	：	4/10/2008 11:38:56 AM
//	功能描述	：
//
// -------------------------------------------------------------------------- //

#include "stdafx.h"
#include "domaccept.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

XmlElement::~XmlElement()
{

}

// 不支持 XPath，只是在子节点中寻找第一个符合名字的 element
ref<XmlElement> XmlElement::SelectSingleNode(stringx nameNode)
{
	stringx _nameNode(nameNode);
	for (size_t i = 0; i < m_subNodes->Count(); ++i)
	{
		ref<XmlNode> subNode = m_subNodes->at(i);
		if (subNode->get_NodeType() != XMLNODE_ELEMENT)
			continue;

		if (subNode->get_NodeName() == _nameNode)
			return ref_cast<XmlElement>(subNode);
	}
	return NULL;
}

// 不支持 XPath，只是在子节点中寻找符合名字的 element
void XmlElement::SelectNodes(stringx nameNode, ref<ArrayList<XmlElement> > items)
{
	items->Clear();

	stringx _nameNode(nameNode);
	for (size_t i = 0; i < m_subNodes->Count(); ++i)
	{
		ref<XmlNode> subNode = m_subNodes->at(i);
		if (subNode->get_NodeType() != XMLNODE_ELEMENT)
			continue;

		if (subNode->get_NodeName() == _nameNode)
			items->Add(ref_cast<XmlElement>(subNode));
	}
}

ref<XmlAttribute> XmlElement::get_Attribute(stringx nameAttr)
{
	stringx	name(nameAttr);
	for (size_t i = 0; i < m_attributes->Count(); ++i)
	{
		ref<XmlAttribute>	attr = m_attributes->at(i);
		if (attr->get_Name() == name)
			return attr;
	}
	return NULL;
}

stringx XmlElement::get_ValueText()
{
	stringx ss;
	for (size_t i = 0; i < m_subNodes->Count(); ++i)
		//XmlNodeCollection::iterator it = m_subNodes.begin(); it != m_subNodes.end(); ++it)
	{
		ref<XmlNode> subNode = m_subNodes->at(i);
		if (subNode->get_NodeType() == XMLNODE_VALUE)
			ss += (ref_cast<XmlNodeValue>(subNode))->get_Value();
	}
	return ss;
}

void XmlElement::AppendNode(ref<XmlNode> subNode)
{
	m_subNodes->Add(subNode);
}

void XmlElement::InsertNodeAt(size_t index, ref<XmlNode> subNode)
{
	ASSERT(index <= m_subNodes->Count());
	m_subNodes->Insert(index, subNode);
}

void XmlElement::SetAttribute(stringx nameAttr, stringx value)
{
	ref<XmlAttribute>	attr = get_Attribute(nameAttr);
	if (attr != NULL)
	{
		attr->set_Value(value);
		return;
	}

	attr = gc_new<XmlAttribute>(nameAttr, value);
	AddAttribute(attr);
}

void XmlElement::RemoveChild(stringx oldchild)
{
	size_t first = -1;
	size_t last = 0;

	for (size_t i = 0; i < m_subNodes->Count(); ++i)
	{
		ref<XmlNode> subNode = m_subNodes->at(i);
		if (subNode->get_NodeType() != XMLNODE_ELEMENT)
			continue;

		if (subNode->get_NodeName() == oldchild)
		{
			if (first == -1)
				first = i;
			last = i;
		}
	}

	if (first != -1)
		m_subNodes->RemoveRange(first, last + 1);
}

// -------------------------------------------------------------------------- //

void XmlDocument::LoadXml(stringx xmlText)
{
	Close();

	ref<_XmlDomAccepter>	accpt = gc_new<_XmlDomAccepter>(this);
	XmlReader::ParseText(accpt, xmlText);
}

void XmlDocument::Load(ref<TextReader> tr)
{
	Close();

	ref<_XmlDomAccepter>	accpt = gc_new<_XmlDomAccepter>(this);
	XmlReader::Parse(accpt, tr);
}

void XmlDocument::Load(ref<Stream> stream)
{
	Close();

	ref<_XmlDomAccepter>	accpt = gc_new<_XmlDomAccepter>(this);
	XmlReader::Parse(accpt, stream);
}

void XmlDocument::Load(stringx filename)
{
	Close();

	ref<_XmlDomAccepter>	accpt = gc_new<_XmlDomAccepter>(this);
	XmlReader::Parse(accpt, filename);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
