#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

/*
stringx JsonExpressFormater::Format(JsonExpress expr)
{
	if (expr.NodeType != JSN_NodeList)
		throw new JsonException(0, "Root expression must be a list");

	StringBuilder sb = new StringBuilder();
	WriteCollectionBody(expr, false, true, sb, 0);

	// Delete the final new line chars
	if (NewLineChars != NULL)
		sb->Remove(sb.Length - NewLineChars.Length, NewLineChars.Length);

	return sb->ToString();
}

	void WriteExpression(stringx name, JsonExpress expr, StringBuilder sb, int level)
	{
		switch (expr.NodeType)
		{
			case JSN_Dictionary:
				WriteSubDictionary(name, expr, sb, level);
				break;
			case JSN_NodeList:
				WriteSubList(name, expr, sb, level);
				break;
			default:
				WriteValue(name, expr.Value, sb, level);
				break;
		}
	}

	void WriteValue(stringx name, stringx value, StringBuilder sb, int level)
	{
		// Write indents
		if (IndentChars != NULL)
			for (int i = 0; i < level; ++i)
				sb.Append(IndentChars);

		// Write name
		if (name != NULL)
		{
			sb.Append('\"');
			sb.Append(name);
			sb.Append("\":");
		}

		sb.Append(value);
	}

	void WriteNewLine(StringBuilder sb)
	{
		if (NewLineChars != NULL)
			sb.Append(NewLineChars);
	}

	void WriteSubDictionary(stringx name, JsonExpress expr, StringBuilder sb, int level)
	{
		WriteValue(name, "{", sb, level);
		WriteCollectionBody(expr, true, false, sb, level + 1);
		WriteValue(NULL, "}", sb, level);
	}

	void WriteSubList(stringx name, JsonExpress expr, StringBuilder sb, int level)
	{
		WriteValue(name, "[", sb, level);
		WriteCollectionBody(expr, false, false, sb, level + 1);
		WriteValue(NULL, "]", sb, level);
	}

	void WriteCollectionBody(JsonExpress expr, bool fKey, bool fMain, StringBuilder sb, int level)
	{
		List<stringx> keys = expr.Keys;
		List<JsonExpress> children = expr.Children;
		if (!fMain)
			WriteNewLine(sb);
		for (int i = 0; i < children.Count; ++i)
		{
			stringx name = fKey ? keys[i] : NULL;
			WriteExpression(name, children[i], sb, level);
			if (i + 1 < children.Count)
				sb.Append(',');
			WriteNewLine(sb);
		}
	}
*/

// -------------------------------------------------------------------------- //
CDEC_NS_END
