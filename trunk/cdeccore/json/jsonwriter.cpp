#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

stringx JsonExpressFormater::Format(ref<JsonNode> node)
{
	ref<StringBuilder> sb = gc_new<StringBuilder>();
	WriteExpression(NULL, node, sb, 0);

	// Delete the final new line chars
	//if (NewLineChars != NULL)
	//	sb->Remove(sb->Length() - NewLineChars.Length(), NewLineChars.Length());

	return sb->ToString();
}

void JsonExpressFormater::WriteExpression(stringx name, ref<JsonNode> node, ref<StringBuilder> sb, int level)
{
	switch (node->GetType())
	{
	case JSN_String:
		WriteValue(name, '\"' + node->TextValue() + '\"', sb, level);
		break;
	case JSN_Integer:
		WriteValue(name, Converter::ToString(node->IntValue()), sb, level);
		break;
	case JSN_Boolean:
		WriteValue(name, node->BoolValue() ? __X("true") : __X("false"), sb, level);
		break;
	case JSN_None:
		WriteValue(name, __X("null"), sb, level);
		break;
	case JSN_Dictionary:
		WriteSubDictionary(name, node, sb, level);
		break;
	case JSN_NodeList:
		WriteSubList(name, node, sb, level);
		break;
	default:
		cdec_throw(JsonException(EC_JSON_NotImplemented, 0));
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

void JsonExpressFormater::WriteSubDictionary(stringx name, ref<JsonNode> node, ref<StringBuilder> sb, int level)
{
	WriteValue(name, __X("{"), sb, level);
	WriteNewLine(sb);

	ref<JsonNode::JsonNodeDictionary> dict = node->NodeDictionary();
	int count = dict->Count();
	foreach (ref<JsonNode>, subnode, dict)
	{
		ASSERT(subnode->GetName() != NULL);
		WriteExpression(subnode->GetName(), subnode, sb, level + 1);
		if (--count > 0)
			sb->Append(',');
		WriteNewLine(sb);
	}
	ASSERT(count == 0);

	WriteValue(NULL, __X("}"), sb, level);
}

void JsonExpressFormater::WriteSubList(stringx name, ref<JsonNode> node, ref<StringBuilder> sb, int level)
{
	WriteValue(name, __X("["), sb, level);
	WriteNewLine(sb);

	ref<JsonNode::JsonNodeList> list = node->NodeList();
	int count = list->Count();
	foreach (ref<JsonNode>, subnode, list)
	{
		ASSERT(subnode->GetName() == NULL);
		WriteExpression(NULL, subnode, sb, level + 1);
		if (--count > 0)
			sb->Append(',');
		WriteNewLine(sb);
	}

	WriteValue(NULL, __X("]"), sb, level);
}

// -------------------------------------------------------------------------- //

JsonWriter::JsonWriter(ref<JsonNode> root, bool fReopen)
{
	Reset();
	m_root = root;
	if (fReopen && (root->GetType() == JSN_Dictionary || root->GetType() == JSN_NodeList))
	{
		m_top = m_root;
		m_stack->Push(m_top);
	}
}

void JsonWriter::Reset()
{
	m_root = m_top = NULL;
	m_stack = gc_new< Stack<JsonNode> >();
}

void JsonWriter::WriteNode(stringx name, ref<JsonNode> node)
{
	if (m_root != NULL)
	{
		// Add a child node under a list or dictionary node
		if (m_top == NULL)
			cdec_throw(JsonException(EC_JSON_WrongCollectionType, 0));

		ASSERT(!m_stack->Empty());
		node->SetName(name);
		m_top->AddChildItem(node);
	}
	else
	{
		// The root element
		ASSERT(m_stack->Empty() && m_top == NULL);
		if (name != NULL)
			cdec_throw(JsonException(EC_JSON_MustNotHaveName, 0));

		m_root = node;
	}
}

void JsonWriter::BeginDictionary(stringx name)
{
	ref<JsonNode> node = JsonNode::NewDictionaryNode();
	WriteNode(name, node);
	m_stack->Push(m_top = node);
}

void JsonWriter::EndDictionary()
{
	if (m_top == NULL || m_top->GetType() != JSN_Dictionary)
		cdec_throw(JsonException(EC_JSON_NoMatchedDictionary, 0));

	m_stack->Pop();
	m_top = m_stack->Empty() ? NULL : m_stack->Peek();
}

void JsonWriter::BeginList(stringx name)
{
	ref<JsonNode> node = JsonNode::NewListNode();
	WriteNode(name, node);
	m_stack->Push(m_top = node);	
}

void JsonWriter::EndList()
{
	if (m_top == NULL || m_top->GetType() != JSN_NodeList)
		cdec_throw(JsonException(EC_JSON_NoMatchedList, 0));

	m_stack->Pop();
	m_top = m_stack->Empty() ? NULL : m_stack->Peek();		
}

ref<JsonNode> JsonWriter::Complete()
{
	if (m_top != NULL)
		cdec_throw(JsonException(EC_JSON_NodeUnclosed, 0));
	if (m_root == NULL)
		cdec_throw(JsonException(EC_JSON_NoRootNode, 0));

	ASSERT(m_stack->Empty());
	return m_root;
}

stringx JsonWriter::GetString()
{
	ref<JsonNode> root = Complete();
	ref<JsonExpressFormater> jsf = gc_new<JsonExpressFormater>();
	jsf->IndentChars = IndentChars;
	jsf->NewLineChars = NewLineChars;
	return jsf->Format(root);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
