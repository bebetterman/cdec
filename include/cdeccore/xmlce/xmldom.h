// -------------------------------------------------------------------------- //
//	文件名		：	xmldom.h
//	创建者		：	杨钢
//	创建时间	：	4/10/2008 11:38:56 AM
//	功能描述	：
//
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class _XmlDomAccepter;	// Internal use

enum XmlNodeType
{
	XMLNODE_VALUE,
	XMLNODE_ELEMENT,
};

class CDECCOREEXPORT XmlAttribute : public Object
{
	DECLARE_REF_CLASS(XmlAttribute)

protected:
	stringx		m_name;
	stringx		m_value;

	XmlAttribute(stringx name, stringx value): m_name(name), m_value(value) {}

	friend class _XmlDomAccepter;

public:
	stringx	get_Name() const { return m_name; }
	stringx	get_Value() const { return m_value; }
	void set_Value(stringx value) { ASSERT(value != NULL); m_value = value; }
};

class CDECCOREEXPORT XmlNode : public Object
{
	DECLARE_REF_CLASS(XmlNode)

public:
	virtual XmlNodeType	get_NodeType() const = 0;
	virtual stringx get_NodeName() const = 0;

	virtual ~XmlNode() {}
};

class CDECCOREEXPORT XmlNodeValue: public XmlNode
{
protected:
	stringx	m_value;

	XmlNodeValue(stringx value): m_value(value) {}

	friend class _XmlDomAccepter;
	friend class XmlDocument;

public:
	XmlNodeType	get_NodeType() const { return XMLNODE_VALUE; }
	stringx	get_NodeName() const { static stringx name(__X("#text")); return name; }
	stringx	get_Value() const { return m_value; }

	~XmlNodeValue() {}
};

typedef ref<ArrayList<XmlNode> >		XmlNodeCollection;
typedef ref<ArrayList<XmlAttribute> >	XmlAttributeCollection;

class CDECCOREEXPORT XmlElement: public XmlNode
{
protected:
	stringx			m_nodeName;
	XmlNodeCollection		m_subNodes;
	XmlAttributeCollection	m_attributes;

	XmlElement(stringx nodeName): m_nodeName(nodeName)
	{
		m_subNodes = gc_new<ArrayList<XmlNode> >();
		m_attributes = gc_new<ArrayList<XmlAttribute> >();
	}

	void	AddChild(ref<XmlNode> pChild)
	{
		m_subNodes->Add(pChild);
	}
	void	AddAttribute(ref<XmlAttribute> pAttribute)
	{
		m_attributes->Add(pAttribute);
	}

	friend class _XmlDomAccepter;
	friend class XmlDocument;

public:
	XmlNodeType			get_NodeType() const { return XMLNODE_ELEMENT; }
	stringx				get_NodeName() const { return m_nodeName; }
	XmlNodeCollection	get_SubNodes() { return m_subNodes; }
	XmlAttributeCollection	get_Attributes() { return m_attributes; }

	ref<XmlElement>		SelectSingleNode(stringx nameNode);
	void				SelectNodes(stringx nameNode, ref<ArrayList<XmlElement> > items);

	ref<XmlAttribute>	get_Attribute(stringx nameAttr);
	stringx				get_AttributeValue(stringx nameAttr) { return get_Attribute(nameAttr)->get_Value(); }
	stringx				get_ValueText();

	void				AppendNode(ref<XmlNode> subNode);
	void				InsertNodeAt(size_t index, ref<XmlNode> subNode);

	void				SetAttribute(stringx nameAttr, stringx value);

	void				RemoveChild(stringx oldchild);

	~XmlElement();
};

class CDECCOREEXPORT XmlDocument : public Object
{
	DECLARE_REF_CLASS(XmlDocument)

protected:
	ref<XmlElement>		m_pRoot;

	void	set_Root(ref<XmlElement> pRoot)
	{
		ASSERT(m_pRoot == NULL);
		m_pRoot = pRoot;
	}

	friend class _XmlDomAccepter;

public:
	XmlDocument() { m_pRoot = NULL; }
	~XmlDocument() { }

	ref<XmlElement>	get_DocumentElement() const { return m_pRoot; }

	void	LoadXml(stringx xmlText);
	void	Load(ref<TextReader> tr);
	void	Load(ref<Stream> stream);
	void	Load(stringx filename);

	void	Close()
	{
		m_pRoot = NULL;
	}

	// ref<XmlAttribute> CreateAttribute(stringx nameAttr, stringx value);
	ref<XmlElement> CreateElement(stringx nameNode) { return gc_new<XmlElement>(nameNode); }
	ref<XmlNodeValue> CreateValueNode(stringx value) { return gc_new<XmlNodeValue>(value); }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
