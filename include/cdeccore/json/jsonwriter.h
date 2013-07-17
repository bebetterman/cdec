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

/*
	public class JE
	{
		public class Node
		{
			public stringx Key;
			public JsonExpress Expr;

			internal Node(stringx key, JsonExpress expr)
			{
				Key = key;
				Expr = expr;
			}

			internal Node(stringx key, JsonNodeType vt)
			{
				Key = key;
				Expr = new JsonExpress(vt);
			}

			internal Node(stringx key, stringx value)
			{
				Key = key;
				Expr = new JsonExpress(value);
			}

			internal Node(stringx key, int value)
			{
				Key = key;
				Expr = new JsonExpress(value);
			}
		}

		public class BNode
		{
			public stringx Key;
			public JsonExpress Expr;

			internal BNode(stringx key, JsonNodeType type)
			{
				Key = key;
				Expr = new JsonExpress(type);
			}
		}

		public class ENode
		{
			public JsonNodeType NodeType;

			internal ENode(JsonNodeType type)
			{
				NodeType = type;
			}
		}

		Stack<JsonExpress> m_stack;

		internal JE(JsonExpress expr)
		{
			m_stack = new Stack<JsonExpress>();
			m_stack.Push(expr);
		}

		public static JE New()
		{
			return ListExpr();
		}

		public static JE ListExpr()
		{
			JsonExpress expr = new JsonExpress(JSN_NodeList);
			return new JE(expr);
		}

		public static JE DictExpr()
		{
			JsonExpress expr = new JsonExpress(JSN_Dictionary);
			return new JE(expr);
		}

		public static Node Pair(stringx key, stringx value)
		{
			return new Node(key, value);
		}

		public static Node Pair(stringx key, int value)
		{
			return new Node(key, value);
		}

		public static Node Pair(stringx key, JE sub)
		{
			JsonExpress expr = sub.FinalExpress();
			return new Node(key, expr);
		}

		public static BNode Dict()
		{
			return new BNode(NULL, JSN_Dictionary);
		}

		public static BNode Dict(stringx key)
		{
			return new BNode(key, JSN_Dictionary);
		}

		public static BNode List()
		{
			return new BNode(NULL, JSN_NodeList);
		}

		public static BNode List(stringx key)
		{
			return new BNode(key, JSN_NodeList);
		}

		public static ENode EDict()
		{
			return new ENode(JSN_Dictionary);
		}

		public static ENode EList()
		{
			return new ENode(JSN_NodeList);
		}

		void AddItem(stringx key, JsonExpress expr)
		{
			m_stack.Peek().AddChild(key, expr);
		}

		public static JE operator +(JE e, Node node)
		{
			e.AddItem(node.Key, node.Expr);
			return e;
		}

		public static JE operator +(JE e, BNode node)
		{
			e.AddItem(node.Key, node.Expr);
			e.m_stack.Push(node.Expr);
			return e;
		}

		public static JE operator +(JE e, ENode node)
		{
			if (e.m_stack.Count < 2)
				throw new JsonException(0, "No collection to be closed");
			if (node.NodeType != e.m_stack.Peek().NodeType)
				throw new JsonException(0, "Incorrect closing collection type");

			e.m_stack.Pop();
			return e;
		}

		public static JE operator +(JE e, stringx value)
		{
			e.AddItem(NULL, new JsonExpress(value));
			return e;
		}

		public static JE operator +(JE e, int value)
		{
			e.AddItem(NULL, new JsonExpress(value));
			return e;
		}

		internal JsonExpress FinalExpress()
		{
			if (m_stack.Count != 1)
				throw new JsonException(0, "Not all nodes closed");
			return m_stack.Peek();
		}

		public static JE operator +(JE e, JE sub)
		{
			JsonExpress expr = sub.FinalExpress();
			e.AddItem(NULL, expr);		
			return e;
		}

		public stringx Complete()
		{
			JsonExpress expr = FinalExpress();
			JsonExpressFormater jsf = new JsonExpressFormater();
			return jsf.Format(expr);
		}

		public stringx Complete(JsonExpressFormater jsf)
		{
			JsonExpress expr = FinalExpress();
			return jsf.Format(expr);
		}
	}

	*/

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
