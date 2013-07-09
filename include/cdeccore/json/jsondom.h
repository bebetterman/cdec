#pragma once
#include <cctype>

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

enum JsonExceptionCode
{
	EC_JSON_ST = MAKE_EX_CODE(ECNS_JSON, 0),
	EC_ExpectContent,			// Except content
	EC_UnexpectedSymbol,
	EC_ExpectSymbol,
	EC_Expect,
	EC_Unexpected,
	EC_EmptyArg,
};

struct JsonException: Exception
{
	int Position;
	stringx	Message;

	JsonException(int code, int pos, stringx message): Exception(code), Position(pos), Message(message)
	{

	}
};

enum JsonNodeType
{
	JSN_None,			// null
	JSN_String,			// string value
	JSN_Integer,		// integer value
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
	JsonNode(): NodeType(JSN_None), IntValue(0), DblValue(0.0), 
		Name(__X("")) , TextValue(__X(""))
	{
		NodeDict = gc_new<JsonNodeDictionary>();
		NodeList = gc_new<JsonNodeList>();
	}
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
CDEC_NS_END
