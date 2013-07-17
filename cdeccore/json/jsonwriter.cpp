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
CDEC_NS_END
