#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

enum JsonExceptionCode
{
	EC_JSON_ST = MAKE_EX_CODE(ECNS_JSON, 0),
	EC_JSON_ExpectContent,		// Except content
	EC_JSON_ExpectEndDictionry,
	EC_JSON_ExpectEndList,
	EC_JSON_ExpectEndValue,
	EC_JSON_EmptyKey,
	EC_JSON_ExpectDQuot,		// Expect "
	EC_JSON_ExpectComma,		// Expect ,
	EC_JSON_ExpectColon,		// Expect :
	EC_JSON_UnexpectedComma,	// Unexpected ,
	EC_JSON_UnexpectedRCB,		// Unexpected } (right curly brace)
	EC_JSON_UnexpectedRSB,		// Unexpected ] (right square bracket)
	EC_JSON_UnexpectedSymbol,	// Unexpected symbol
	EC_JSON_UnexpectedContent,
	EC_JSON_UnexpectedValueToken,
	EC_JSON_UnexpectedNullToken,

	EC_JSON_TypeError,			// Invalid node type
	EC_JSON_MustHaveName,		// Must have a name
	EC_JSON_MustNotHaveName,	// Must not have a name
	EC_JSON_NoRootNode,			// Root node is not set
	EC_JSON_NoMatchedDictionary,// No matched dictionary node
	EC_JSON_NoMatchedList,		// No matched list node
	EC_JSON_NoMatchedCollection,
	EC_JSON_WrongCollectionType,
	EC_JSON_NodeUnclosed,		// Not all nodes closed
	EC_JSON_NotImplemented,		// Not implmeneted
};

struct JsonException: Exception
{
	int Position;
	stringx	Message;

	JsonException(int code, int pos): Exception(code), Position(pos)
	{

	}
};

enum JsonNodeType
{
	JSN_None,			// null
	JSN_String,			// string value
	JSN_Integer,		// 64-bit integer value
	JSN_Float,			// 64-bit IEEE floating-point value
	JSN_Boolean,		// boolean value
	JSN_Dictionary,		// a key-node dictionary
	JSN_NodeList,		// a node list
};

template<class _Map>
class IndexedDictionaryV: public Object
{
	DECLARE_REF_CLASS(IndexedDictionaryV<_Map>)

public:
	typedef typename _Map::key_type		key_type;
	typedef typename _Map::value_type	value_type;
	typedef typename _Map::KeyValuePair	KeyValuePair;
	typedef ArrayListV<key_type>		KeyList;
	
protected:
	ref<KeyList>	m_keylist;
	ref<_Map>		m_map;

	class key_order_value_enum: public IEnumerator<value_type>
	{
		DECLARE_REF_CLASS(key_order_value_enum)

		ref<IEnumerator<key_type> >	_e;
		ref<_Map>		_map;

	public:
		key_order_value_enum(ref<IEnumerator<key_type> > e, ref<_Map> map): _e(e), _map(map) {}

		value_type	Current() { return _e->Valid() ? _map->Get(_e->Current()) : value_type(); }
		bool		Reset() { return _e->Reset(); }
		bool		Valid() { return _e->Valid(); }
		bool		Next() { return _e->Next(); }
	};

public:
	IndexedDictionaryV()
	{
		m_keylist = gc_new<KeyList>();
		m_map = gc_new<_Map>();
	}

	bool Insert(const key_type& key, const value_type& value)
	{
		bool r = m_map->Insert(key, value);
		if (r)
			m_keylist->Add(key);
		return r;
	}

	value_type& at(const key_type& key) { return m_map->at(key); }

	const value_type& Get(const key_type& key) { return m_map->Get(key); }

	int Count() { ASSERT(m_keylist->Count() == m_map->Count()); return m_map->Count(); }

	ref<IEnumerator<value_type> > GetEnumerator()
	{
		ref<IEnumerator<key_type> > e = m_keylist->GetEnumerator();
		return gc_new<key_order_value_enum>(e, m_map);
	}
};

class JsonNode: public Object
{
	DECLARE_REF_CLASS(JsonNode)

public:
	typedef ArrayList<JsonNode>				JsonNodeList;
	typedef SortedMapVR<stringx, JsonNode>	JsonNodeMap;
	typedef IndexedDictionaryV<JsonNodeMap>	JsonNodeDictionary;

protected:
	JsonNodeType	m_type;
	stringx			m_name;
	variant_core	m_value;

private:
	JsonNode() {}

public:
	JsonNode(JsonNodeType type): m_type(type) {}
	~JsonNode() { ClearValue(); }

	inline void	ClearValue();

	JsonNodeType	GetType() { return m_type; }

	stringx	GetName() { return m_name; }
	void	SetName(stringx name) { m_name = name; }

	stringx	TextValue() { ASSERT(m_type == JSN_String); return m_value.GetString(); }

	INT64	IntValue() { ASSERT(m_type == JSN_Integer); return m_value.GetInt64(); }

	double	FloatValue() { ASSERT(m_type == JSN_Float); return m_value.GetDouble(); }

	bool	BoolValue() { ASSERT(m_type == JSN_Boolean); return m_value.GetBool(); }

	ref<JsonNodeList>		NodeList()
	{
		ASSERT(m_type == JSN_NodeList);
		return ref_cast<JsonNodeList>(m_value.GetObject());
	}

	ref<JsonNodeDictionary>	NodeDictionary()
	{
		ASSERT(m_type == JSN_Dictionary);
		return ref_cast<JsonNodeDictionary>(m_value.GetObject());
	}

	ref<JsonNode>	GetChild(int index) { return NodeList()->at(index); }

	ref<JsonNode>	GetChild(stringx key) { return NodeDictionary()->at(key); }
	ref<JsonNode>	TryGetChild(stringx key) { return NodeDictionary()->Get(key); }

	inline int		GetChildrenCount();

	stringx		GetChildTextValue(int index) { return GetChild(index)->TextValue(); }
	stringx		GetChildTextValue(stringx key) { return GetChild(key)->TextValue(); }

	INT64		GetChildIntValue(int index) { return GetChild(index)->IntValue(); }
	INT64		GetChildIntValue(stringx key) { return GetChild(key)->IntValue(); }

	double		GetChildFloatValue(int index) { return GetChild(index)->FloatValue(); }
	double		GetChildFloatValue(stringx key) { return GetChild(key)->FloatValue(); }

	inline void	AddListItem(ref<JsonNode> node);
	inline void	AddDictionaryItem(ref<JsonNode> node);
	inline void AddChildItem(ref<JsonNode> node);

	static ref<JsonNode>	NewStringNode(stringx value)
	{
		ref<JsonNode> node = gc_new<JsonNode>(JSN_String);
		node->m_value.SetString(value);
		return node;
	}

	static ref<JsonNode>	NewIntNode(INT64 value)
	{
		ref<JsonNode> node = gc_new<JsonNode>(JSN_Integer);
		node->m_value.SetInt64(value);
		return node;
	}

	static ref<JsonNode>	NewFloatNode(double value)
	{
		ref<JsonNode> node = gc_new<JsonNode>(JSN_Float);
		node->m_value.SetDouble(value);
		return node;
	}

	static ref<JsonNode>	NewBoolNode(bool value)
	{
		ref<JsonNode> node = gc_new<JsonNode>(JSN_Boolean);
		node->m_value.SetBool(value);
		return node;
	}

	static ref<JsonNode>	NewNullNode() { return gc_new<JsonNode>(JSN_None); }

	static ref<JsonNode>	NewDictionaryNode()
	{
		ref<JsonNode> node = gc_new<JsonNode>(JSN_Dictionary);
		node->m_value.SetObject(gc_new<JsonNodeDictionary>());
		return node;
	}

	static ref<JsonNode>	NewListNode()
	{
		ref<JsonNode> node = gc_new<JsonNode>(JSN_NodeList);
		node->m_value.SetObject(gc_new<JsonNodeList>());
		return node;
	}
};

class CDECCOREEXPORT JsonParser
{
public:
	static ref<JsonNode>	ParseText(stringx text);
};

// -------------------------------------------------------------------------- //

inline void JsonNode::ClearValue()
{
	switch (m_type)
	{
	case JSN_String:
		m_value.ClearString();
		break;
	case JSN_NodeList:
	case JSN_Dictionary:
		m_value.ClearObject();
		break;
	default:
		m_value.ClearLiteral();
		break;
	}
	m_type = JSN_None;
}

inline int JsonNode::GetChildrenCount()
{
	switch (m_type)
	{
	case JSN_NodeList:
		{
			weak_ref<JsonNodeList> list = ref_cast<JsonNodeList>(m_value.GetObject());
			return list->Count();
		}
	case JSN_Dictionary:
		{
			weak_ref<JsonNodeDictionary> dict = ref_cast<JsonNodeDictionary>(m_value.GetObject());
			return dict->Count();
		}
	default:
		return 0;
	}
}

inline void JsonNode::AddListItem(ref<JsonNode> node)
{
	if (m_type != JSN_NodeList)
		cdec_throw(JsonException(EC_JSON_TypeError, 0));
	if (node->GetName() != NULL)
		cdec_throw(JsonException(EC_JSON_MustNotHaveName, 0));

	weak_ref<JsonNodeList> list = ref_cast<JsonNodeList>(m_value.GetObject());
	list->Add(node);
}

inline void JsonNode::AddDictionaryItem(ref<JsonNode> node)
{
	if (m_type != JSN_Dictionary)
		cdec_throw(JsonException(EC_JSON_TypeError, 0));
	if (node->GetName() == NULL)
		cdec_throw(JsonException(EC_JSON_MustHaveName, 0));

	weak_ref<JsonNodeDictionary> dict = ref_cast<JsonNodeDictionary>(m_value.GetObject());
	dict->Insert(node->GetName(), node);
}

inline void JsonNode::AddChildItem(ref<JsonNode> node)
{
	switch (m_type)
	{
	case JSN_NodeList:
		AddListItem(node);
		break;
	case JSN_Dictionary:
		AddDictionaryItem(node);
		break;
	default:
		cdec_throw(JsonException(EC_JSON_TypeError, 0));
	}

}

// -------------------------------------------------------------------------- //
CDEC_NS_END
