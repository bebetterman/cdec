#pragma once
#include <cctype>

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class JsonExpressFormater;

class JsonExpress: public Object
{
	DECLARE_REF_CLASS(JsonExpress)

public:
	JsonNodeType	NodeType;
	stringx			Value;

	ref<StringArrayList>			Keys;
	ref<ArrayList<JsonExpress> >	Children;

public:
	inline JsonExpress(stringx value): NodeType(JSN_String)
	{
		Value = '\"' + value + '\"';
	}

	inline JsonExpress(int value): NodeType(JSN_Integer)
	{
		Value = Converter::ToString(value);
	}

	inline JsonExpress(JsonNodeType type);

	static inline ref<JsonExpress> CreateMain()
	{
		return gc_new<JsonExpress>(JSN_NodeList);
	}

	inline bool IsCollection()
	{
		return NodeType == JSN_Dictionary || NodeType == JSN_NodeList;
	}

	inline void AddChild(stringx name, ref<JsonExpress> value);

	inline stringx Complete();

	inline stringx Complete(ref<JsonExpressFormater> jsf);
};

// -------------------------------------------------------------------------- //

class CDECCOREEXPORT JsonExpressFormater: public Object
{
	DECLARE_REF_CLASS(JsonExpressFormater)

public:
	stringx	IndentChars;
	stringx	NewLineChars;

public:
	stringx Format(ref<JsonExpress> expr);

protected:
	void WriteExpression(stringx name, ref<JsonExpress> expr, ref<StringBuilder> sb, int level);
	void WriteValue(stringx name, stringx value, ref<StringBuilder> sb, int level);
	void WriteNewLine(ref<StringBuilder> sb);
	void WriteSubDictionary(stringx name, ref<JsonExpress> expr, ref<StringBuilder> sb, int level);
	void WriteSubList(stringx name, ref<JsonExpress> expr, ref<StringBuilder> sb, int level);
	void WriteCollectionBody(ref<JsonExpress> expr, bool fKey, bool fMain, ref<StringBuilder> sb, int level);
};

// -------------------------------------------------------------------------- //

class CDECCOREEXPORT JsonWriter: public Object
{
	DECLARE_REF_CLASS(JsonWriter)

public:
	stringx		IndentChars;
	stringx		NewLineChars;

protected:
	ref<JsonExpress>			m_expr;
	ref<Stack<JsonExpress> >	m_stack;

public:
	JsonWriter() { Reset(); }
	
	void Reset();

	void WriteString(stringx name, stringx value) { m_expr->AddChild(name, gc_new<JsonExpress>(value)); }
	void WriteInt(stringx name, int value) { m_expr->AddChild(name, gc_new<JsonExpress>(value)); }

	void BeginDictionary(stringx name);
	void EndDictionary();

	void BeginList(stringx name);
	void EndList();

	stringx Complete();
};

// -------------------------------------------------------------------------- //
namespace json_express {

// Note: JE is not a Cdec class
class JE
{
public:
	struct Node
	{
		stringx				Key;
		ref<JsonExpress>	Expr;

		Node(stringx key, ref<JsonExpress> expr)
		{
			Key = key;
			Expr = expr;
		}

		Node(stringx key, JsonNodeType vt)
		{
			Key = key;
			Expr = gc_new<JsonExpress>(vt);
		}

		Node(stringx key, stringx value)
		{
			Key = key;
			Expr = gc_new<JsonExpress>(value);
		}

		Node(stringx key, int value)
		{
			Key = key;
			Expr = gc_new<JsonExpress>(value);
		}
	};

	struct BNode
	{
		stringx				Key;
		ref<JsonExpress>	Expr;

		BNode(stringx key, JsonNodeType type)
		{
			Key = key;
			Expr = gc_new<JsonExpress>(type);
		}
	};

	struct ENode
	{
		JsonNodeType		NodeType;

		ENode(JsonNodeType type)
		{
			NodeType = type;
		}
	};
	
	ref< Stack<JsonExpress> >	m_stack;

	JE(ref<JsonExpress> expr)
	{
		m_stack = gc_new< Stack<JsonExpress> >();
		m_stack->Push(expr);
	}

	static JE New()
	{
		return ListExpr();
	}

	static JE ListExpr()
	{
		ref<JsonExpress> expr = gc_new<JsonExpress>(JSN_NodeList);
		return JE(expr);
	}

	static JE DictExpr()
	{
		ref<JsonExpress> expr = gc_new<JsonExpress>(JSN_Dictionary);
		return JE(expr);
	}

	static Node Pair(stringx key, stringx value)
	{
		return Node(key, value);
	}

	static Node Pair(stringx key, int value)
	{
		return Node(key, value);
	}

	static Node Pair(stringx key, JE sub)
	{
		ref<JsonExpress> expr = sub.FinalExpress();
		return Node(key, expr);
	}

	static BNode Dict()
	{
		return BNode(NULL, JSN_Dictionary);
	}

	static BNode Dict(stringx key)
	{
		return BNode(key, JSN_Dictionary);
	}

	static BNode List()
	{
		return BNode(NULL, JSN_NodeList);
	}

	static BNode List(stringx key)
	{
		return BNode(key, JSN_NodeList);
	}

	static ENode EDict()
	{
		return ENode(JSN_Dictionary);
	}

	static ENode EList()
	{
		return ENode(JSN_NodeList);
	}

	JE& operator +(Node node)
	{
		AddItem(node.Key, node.Expr);
		return *this;
	}

	JE& operator +(BNode node)
	{
		AddItem(node.Key, node.Expr);
		m_stack->Push(node.Expr);
		return *this;
	}

	JE& operator +(ENode node)
	{
		if (m_stack->Count() < 2)
			cdec_throw(JsonException(EC_JSON_NoMatchedCollection, 0));
		if (node.NodeType != m_stack->Peek()->NodeType)
			cdec_throw(JsonException(EC_JSON_WrongCollectionType, 0));

		m_stack->Pop();
		return *this;
	}

	JE& operator +(stringx value)
	{
		AddItem(NULL, gc_new<JsonExpress>(value));
		return *this;
	}

	JE& operator +(int value)
	{
		AddItem(NULL, gc_new<JsonExpress>(value));
		return *this;
	}

	JE& operator +(JE sub)
	{
		ref<JsonExpress> expr = sub.FinalExpress();
		AddItem(NULL, expr);		
		return *this;
	}

	stringx Complete()
	{
		ref<JsonExpress> expr = FinalExpress();
		ref<JsonExpressFormater> jsf = gc_new<JsonExpressFormater>();
		return jsf->Format(expr);
	}

	stringx Complete(ref<JsonExpressFormater> jsf)
	{
		ref<JsonExpress> expr = FinalExpress();
		return jsf->Format(expr);
	}

protected:
	void AddItem(stringx key, ref<JsonExpress> expr)
	{
		m_stack->Peek()->AddChild(key, expr);
	}

	ref<JsonExpress> FinalExpress()
	{
		if (m_stack->Count() != 1)
			cdec_throw(JsonException(EC_JSON_NodeUnclosed, 0));
		return m_stack->Peek();
	}
};

}

// -------------------------------------------------------------------------- //

inline JsonExpress::JsonExpress(JsonNodeType type)
{
	if (type == JSN_Dictionary || type == JSN_NodeList)
	{
		NodeType = type;
		Keys = gc_new<StringArrayList>();
		Children = gc_new<ArrayList<JsonExpress> >();
	}
	else
		cdec_throw(JsonException(EC_JSON_TypeError, 0));
}

inline void JsonExpress::AddChild(stringx name, ref<JsonExpress> value)
{
	if (NodeType == JSN_NodeList)
	{
		if (name != NULL)
			cdec_throw(JsonException(EC_JSON_MustNotHaveName, 0));
		Children->Add(value);
	}
	else if (NodeType == JSN_Dictionary)
	{
		if (name == NULL)
			cdec_throw(JsonException(EC_JSON_MustHaveName, 0));
		Keys->Add(name);
		Children->Add(value);
	}
	else
		cdec_throw(JsonException(EC_JSON_TypeError, 0));
}

inline stringx JsonExpress::Complete()
{
	ref<JsonExpressFormater> jsf = gc_new<JsonExpressFormater>();
	return jsf->Format(this);
}

inline stringx JsonExpress::Complete(ref<JsonExpressFormater> jsf)
{
	return jsf->Format(this);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
