#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class JsonParserImpl
{
public:
	static ref<JsonNode> ParseText(stringx text);

protected:
	static ref<JsonNode> ParseInnerTextVariant(stringx text, int& pos);

	static ref<JsonNode> ParseInnerTextDictionary(stringx text, int& pos);
	static ref<JsonNode> ParseInnerTextList(stringx text, int& pos);
	static ref<JsonNode> ParseInnerTextStringValue(stringx text, int& pos);
	static ref<JsonNode> ParseInnerTextNumberValue(stringx text, int& pos);
	static ref<JsonNode> ParseInnerTextBooleanValue(stringx text, int& pos);
	static ref<JsonNode> ParseInnerTextNoneValue(stringx text, int& pos);

	static stringx ParseStringValueToken(stringx text, int& pos);

	static bool ParseNumberValueToken(stringx text, int& pos, INT64& ivalue, double& fvalue);
	static INT64 ParseNumberValueIntPart(stringx text, int& pos);
	static double ParseNumberValueDecimalPart(stringx text, int& pos);

	static bool ParseBooleanValueToken(stringx text, int& pos);
	static void ParseNoneValueToken(stringx text, int& pos);

	static int SeekNextNotSpace(stringx text, int pos);
};

// -------------------------------------------------------------------------- //

ref<JsonNode> JsonParserImpl::ParseText(stringx text)
{
	int pos = 0;
	ref<JsonNode> root = ParseInnerTextVariant(text, pos);

	if (pos >= 0 && pos < text.Length())
	{
		pos = SeekNextNotSpace(text, pos);
		if (pos >= 0)
			cdec_throw(JsonException(EC_JSON_UnexpectedContent, pos));
	}

	return root;
}

ref<JsonNode> JsonParserImpl::ParseInnerTextVariant(stringx text, int& pos)
{
	pos = SeekNextNotSpace(text, pos);
	if (pos < 0)
		cdec_throw(JsonException(EC_JSON_ExpectContent, pos));

	WCHAR ch = text[pos];
	if (ch == '{')
		return ParseInnerTextDictionary(text, pos);
	else if (ch == '[')
		return ParseInnerTextList(text, pos);
	else if (ch == '\"')
		return ParseInnerTextStringValue(text, pos);
	else if ((ch >= '0' && ch <= '9') || (ch == '+' || ch == '-'))
		return ParseInnerTextNumberValue(text, pos);
	else if (ch == 'T' || ch == 't' || ch == 'F' || ch == 'f')
		return ParseInnerTextBooleanValue(text, pos);
	else if (ch == 'N' || ch == 'n')
		return ParseInnerTextNoneValue(text, pos);
	else
		cdec_throw(JsonException(EC_JSON_UnexpectedSymbol, pos));
}

ref<JsonNode> JsonParserImpl::ParseInnerTextStringValue(stringx text, int& pos)
{
	ASSERT(text[pos] == '\"');
	stringx value = ParseStringValueToken(text,  pos);
	return JsonNode::NewStringNode(value);
}

ref<JsonNode> JsonParserImpl::ParseInnerTextNumberValue(stringx text, int& pos)
{
	bool fPos = true;
	switch (text[pos])
	{
	case '+':
		++pos;
		break;
	case '-':
		fPos = false;
		++pos;
		break;
	}

	ASSERT(text[pos] >= '0' && text[pos] <= '9');
	INT64 intV = 0;
	double dblV = 0.0;
	bool fFloat = ParseNumberValueToken(text, pos, intV, dblV);
	if (fFloat)
		return JsonNode::NewFloatNode(fPos ? dblV : -dblV);
	else
		return JsonNode::NewIntNode(fPos ? intV : -intV);
}

ref<JsonNode> JsonParserImpl::ParseInnerTextBooleanValue(stringx text, int& pos)
{
	bool value = ParseBooleanValueToken(text, pos);
	return JsonNode::NewBoolNode(value);
}

ref<JsonNode> JsonParserImpl::ParseInnerTextNoneValue(stringx text, int& pos)
{
	ParseNoneValueToken(text, pos);
	return JsonNode::NewNullNode();
}

// -------------------------------------------------------------------------- //

ref<JsonNode> JsonParserImpl::ParseInnerTextDictionary(stringx text, int& pos)
{
	ASSERT(text[pos] == '{');

	ref<JsonNode> node = JsonNode::NewDictionaryNode();
	++pos;
	int flag = 0;	// last symbol: 0 init; 1 comma, 2 content

	while (true)
	{
		pos = SeekNextNotSpace(text, pos);
		if (pos < 0)
			cdec_throw(JsonException(EC_JSON_ExpectEndDictionry, pos));

		WCHAR ch = text[pos];
		if (ch == '\"')
		{
			// Parse key part
			if (flag == 2)
				cdec_throw(JsonException(EC_JSON_ExpectComma, pos));

			stringx key = ParseStringValueToken(text, pos);
			if (key.Length() == 0)
				cdec_throw(JsonException(EC_JSON_EmptyKey, pos));

			pos = SeekNextNotSpace(text, pos);
			if (pos < 0 || text[pos] != ':')
				cdec_throw(JsonException(EC_JSON_ExpectColon, pos));
			++pos;

			// Parse value part			
			ref<JsonNode> subnode = ParseInnerTextVariant(text, pos);
			subnode->SetName(key);
			node->AddDictionaryItem(subnode);

			flag = 2;
		}
		else if (ch == ',')
		{
			if (flag != 2)
				cdec_throw(JsonException(EC_JSON_UnexpectedComma, pos));
			++pos;
			flag = 1;
		}
		else if (ch == '}')
		{
			if (flag == 1)
				cdec_throw(JsonException(EC_JSON_UnexpectedRCB, pos));
			++pos;
			break;
		}
		else
			cdec_throw(JsonException(EC_JSON_UnexpectedSymbol, pos));
	}

	return node;
}

ref<JsonNode> JsonParserImpl::ParseInnerTextList(stringx text, int& pos)
{
	ASSERT(text[pos] == '[');

	ref<JsonNode> node = JsonNode::NewListNode();
	++pos;
	int flag = 0;	// last symbol: 0 init; 1 comma, 2 content

	while (true)
	{
		pos = SeekNextNotSpace(text, pos);
		if (pos < 0)
			cdec_throw(JsonException(EC_JSON_ExpectEndList, pos));

		WCHAR ch = text[pos];
		if (ch == ']')
		{
			if (flag == 1)
				cdec_throw(JsonException(EC_JSON_UnexpectedRSB, pos));
			++pos;
			break;
		}
		else if (ch == ',')
		{
			if (flag != 2)
				cdec_throw(JsonException(EC_JSON_UnexpectedComma, pos));
			++pos;
			flag = 1;
		}
		else
		{
			if (flag == 2)
				cdec_throw(JsonException(EC_JSON_ExpectComma, pos));

			ref<JsonNode> subnode = ParseInnerTextVariant(text, pos);
			node->AddListItem(subnode);
			flag = 2;
		}
	}

	return node;
}

// -------------------------------------------------------------------------- //

stringx JsonParserImpl::ParseStringValueToken(stringx text, int& pos)
{
	ASSERT(text[pos] == '\"');

	ref<StringBuilder> sb = gc_new<StringBuilder>();
	++pos;

	while (pos < text.Length())
	{
		WCHAR ch = text[pos++];
		if (ch == '\\')
		{
			ch = text[pos++];
			switch (ch)
			{
			case '0':
				sb->Append('\0');	// 0
				break;
			case 'a':
				sb->Append('\a');	// 7 BELL
				break;
			case 'b':
				sb->Append('b');		// 8 Backspace
				break;
			case 't':
				sb->Append('\t');	// 9 Table
				break;
			case 'n':
				sb->Append('\n');	// 10 LF
				break;
			case 'v':
				sb->Append('\v');	// 11 FF
				break;
			case 'f':
				sb->Append('\f');	// 12 FF
				break;
			case 'r':
				sb->Append('\r');	// 13 CR
				break;
			case 'u':
				sb->Append((WCHAR)Converter::ToUInt16(text.Substring(pos, 4), 16));
				pos += 4;
				break;
			case '\\':
			case '\'':
			case '\"':
			default:
				sb->Append(ch);
				break;
			}
		}
		else if (ch == '\"')
			return sb->ToString();
		else
			sb->Append(ch);
	}
	cdec_throw(JsonException(EC_JSON_ExpectEndValue, pos));
}

bool JsonParserImpl::ParseNumberValueToken(stringx text, int& pos, INT64& ivalue, double& fvalue)
{
	ivalue = ParseNumberValueIntPart(text, pos);
	if (pos < text.Length() && text[pos] == '.')
	{
		++pos;
		fvalue = ParseNumberValueDecimalPart(text, pos) + ivalue;
		return true;
	}
	return false;
}

INT64 JsonParserImpl::ParseNumberValueIntPart(stringx text, int& pos)
{
	ASSERT(text[pos] >= '0' && text[pos] <= '9');
	INT64 value = 0;
	while (pos < text.Length())
	{
		WCHAR ch = text[pos];
		if (ch >= '0' && ch <= '9')
			value = value * 10 + (ch - '0');
		else
			break;

		++pos;
	}
	return value;
}

double JsonParserImpl::ParseNumberValueDecimalPart(stringx text, int& pos)
{
	ASSERT(text[pos] >= '0' && text[pos] <= '9');

	double value = 0.0, prec = 0.1;
	while (pos < text.Length())
	{
		WCHAR ch = text[pos];
		if (ch >= '0' && ch <= '9')
			value += (ch - '0') * prec;
		else
			break;

		++pos;
		prec *= 0.1;
	}
	return value;
}

bool JsonParserImpl::ParseBooleanValueToken(stringx text, int& pos)
{
	int endpos = pos;
	while (endpos < text.Length())
	{
		WCHAR ch = text[endpos];
		if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
			break;
		++endpos;
	}

	stringx token = text.Substring(pos, endpos - pos).ToLower();
	pos = endpos;
	if (token == __X("true"))
		return true;
	else if (token == __X("false"))
		return false;
	else 
		cdec_throw(JsonException(EC_JSON_UnexpectedValueToken, pos));
}

void JsonParserImpl::ParseNoneValueToken(stringx text, int& pos)
{
	int endpos = pos;
	while (endpos < text.Length())
	{
		WCHAR ch = text[endpos];
		if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
			break;
		++endpos;
	}

	stringx token = text.Substring(pos, endpos - pos).ToLower();
	pos = endpos;
	if (token != __X("null"))
		cdec_throw(JsonException(EC_JSON_UnexpectedNullToken, pos));
}

int JsonParserImpl::SeekNextNotSpace(stringx text, int pos)
{
	while (pos < text.Length())
	{
		if (text[pos] > 32)
			return pos;
		else
			++pos;
	}
	return -1;
}

// -------------------------------------------------------------------------- //

ref<JsonNode> JsonParser::ParseText(stringx text)
{
	return JsonParserImpl::ParseText(text);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
