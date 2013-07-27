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
	EC_JSON_IncorrectRoot,		// Root expression must be a list
	EC_JSON_NoMatchedDictionary,// No matched dictionary node
	EC_JSON_NoMatchedList,		// No matched list node
	EC_JSON_NoMatchedCollection,
	EC_JSON_WrongCollectionType,
	EC_JSON_NodeUnclosed,		// Not all nodes closed
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
	JSN_Integer,		// integer value
	JSN_Int64,		// int64 value
	JSN_Float,			// floating-point value
	JSN_Boolean,		// boolean value
	JSN_Dictionary,		// a key-node dictionary
	JSN_NodeList,		// a node list
};

class JsonNode: public Object
{
	DECLARE_REF_CLASS(JsonNode)

public:
	typedef ArrayList<JsonNode>				JsonNodeList;
	typedef SortedMapVR<stringx, JsonNode>	JsonNodeDictionary;

	JsonNodeType	NodeType;
	stringx			Name;
	stringx			TextValue;				// String
	int				IntValue;				// Integer, Boolean(0/1)
	double			DblValue;				// Floating-pointer number

	ref<JsonNodeDictionary>	NodeDict;		// Dictionary
	ref<JsonNodeList>		NodeList;		// List

public:
	JsonNode(): NodeType(JSN_None), IntValue(0), DblValue(0.0)
	{
	}

	ref<JsonNode>	GetChild(int index);
	ref<JsonNode>	GetChild(stringx key);
	int				GetChildrenCount();

	stringx		GetChildTextValue(int index) { return GetChild(index)->TextValue; }
	stringx		GetChildTextValue(stringx key) { return GetChild(key)->TextValue; }

	int			GetChildIntValue(int index) { return GetChild(index)->IntValue; }
	int			GetChildIntValue(stringx key) { return GetChild(key)->IntValue; }

	double		GetChildDblValue(int index) { return GetChild(index)->DblValue; }
	double		GetChildDblValue(stringx key) { return GetChild(key)->DblValue; }
};

class CDECCOREEXPORT JsonDom: public Object
{
	DECLARE_REF_CLASS(JsonDom)

public:
	ref<JsonNode>	Root;

public:
	JsonDom():Root(gc_new<JsonNode>()){}
public:
	void	Load(stringx text);
};

// -------------------------------------------------------------------------- //

inline ref<JsonNode> JsonNode::GetChild(int index)
{
	if (NodeType != JSN_NodeList)
		cdec_throw(JsonException(EC_JSON_TypeError, 0));
	return NodeList->at(index);
}

inline ref<JsonNode> JsonNode::GetChild(stringx key)
{
	if (NodeType != JSN_Dictionary)
		cdec_throw(JsonException(EC_JSON_TypeError, 0));
	return NodeDict->Get(key);
}

inline int JsonNode::GetChildrenCount()
{
	switch (NodeType)
	{
	case JSN_NodeList:
		return NodeList->Count();
	case JSN_Dictionary:
		return NodeDict->Count();
	default:
		return 0;
	}
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
