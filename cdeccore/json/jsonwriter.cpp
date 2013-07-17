#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

stringx JsonExpressFormater::Format(ref<JsonExpress> expr)
{
	if (expr->NodeType != JSN_NodeList)
		cdec_throw(JsonException(EC_JSON_IncorrectRoot, 0));

	ref<StringBuilder> sb = gc_new<StringBuilder>();
	WriteCollectionBody(expr, false, true, sb, 0);

	// Delete the final new line chars
	if (NewLineChars != NULL)
		sb->Remove(sb->Length() - NewLineChars.Length(), NewLineChars.Length());

	return sb->ToString();
}

void JsonExpressFormater::WriteExpression(stringx name, ref<JsonExpress> expr, ref<StringBuilder> sb, int level)
{
	switch (expr->NodeType)
	{
		case JSN_Dictionary:
			WriteSubDictionary(name, expr, sb, level);
			break;
		case JSN_NodeList:
			WriteSubList(name, expr, sb, level);
			break;
		default:
			WriteValue(name, expr->Value, sb, level);
			break;
	}
}

void JsonExpressFormater::WriteValue(stringx name, stringx value, ref<StringBuilder> sb, int level)
{
	// Write indents
	if (IndentChars != NULL)
		for (int i = 0; i < level; ++i)
			sb->Append(IndentChars);

	// Write name
	if (name != NULL)
	{
		sb->Append('\"');
		sb->Append(name);
		sb->Append(__X("\":"));
	}

	sb->Append(value);
}

void JsonExpressFormater::WriteNewLine(ref<StringBuilder> sb)
{
	if (NewLineChars != NULL)
		sb->Append(NewLineChars);
}

void JsonExpressFormater::WriteSubDictionary(stringx name, ref<JsonExpress> expr, ref<StringBuilder> sb, int level)
{
	WriteValue(name, __X("{"), sb, level);
	WriteCollectionBody(expr, true, false, sb, level + 1);
	WriteValue(NULL, __X("}"), sb, level);
}

void JsonExpressFormater::WriteSubList(stringx name, ref<JsonExpress> expr, ref<StringBuilder> sb, int level)
{
	WriteValue(name, __X("["), sb, level);
	WriteCollectionBody(expr, false, false, sb, level + 1);
	WriteValue(NULL, __X("]"), sb, level);
}

void JsonExpressFormater::WriteCollectionBody(ref<JsonExpress> expr, bool fKey, bool fMain, ref<StringBuilder> sb, int level)
{
	ref<StringArrayList> keys = expr->Keys;
	ref<ArrayList<JsonExpress> > children = expr->Children;
	if (!fMain)
		WriteNewLine(sb);
	for (int i = 0, n = children->Count(); i < n; ++i)
	{
		stringx name = fKey ? keys->at(i) : NULL;
		WriteExpression(name, children->at(i), sb, level);
		if (i + 1 < n)
			sb->Append(',');
		WriteNewLine(sb);
	}
}

// -------------------------------------------------------------------------- //

void JsonWriter::Reset()
{
	m_expr = JsonExpress::CreateMain();
	m_stack = gc_new< Stack<JsonExpress> >();
	m_stack->Push(m_expr);
}

void JsonWriter::BeginDictionary(stringx name)
{
	ref<JsonExpress> expr = gc_new<JsonExpress>(JSN_Dictionary);
	m_expr->AddChild(name, expr);
	m_expr = expr;
	m_stack->Push(expr);
}

void JsonWriter::EndDictionary()
{
	if (m_stack->Count() < 2 || m_expr->NodeType != JSN_Dictionary)
		cdec_throw(JsonException(EC_JSON_NoMatchedDictionary, 0));
	m_stack->Pop();
	m_expr = m_stack->Peek();
}

void JsonWriter::BeginList(stringx name)
{
	ref<JsonExpress> expr = gc_new<JsonExpress>(JSN_NodeList);
	m_expr->AddChild(name, expr);
	m_expr = expr;
	m_stack->Push(expr);	
}

void JsonWriter::EndList()
{
	if (m_stack->Count() < 2 || m_expr->NodeType != JSN_NodeList)
		cdec_throw(JsonException(EC_JSON_NoMatchedList, 0));
	m_stack->Pop();
	m_expr = m_stack->Peek();			
}

stringx JsonWriter::Complete()
{
	if (m_stack->Count() != 1)
		cdec_throw(JsonException(EC_JSON_NodeUnclosed, 0));

	ASSERT(m_stack->Peek() == m_expr);
	ref<JsonExpressFormater> jsf = gc_new<JsonExpressFormater>();
	jsf->IndentChars = IndentChars;
	jsf->NewLineChars = NewLineChars;
	return jsf->Format(m_expr);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
