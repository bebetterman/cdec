#include "stdafx.h"

//--------------DEBUG DEFINE--------------------
#define DEBUG_TRUE 1
#define DEBUG_FALSE 0

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class JsonTextParser
{
protected:
	ref<JsonDom>	m_dom;

protected:
	
	int ParseInnerTextVariant(ref<JsonNode> node, stringx text, int pos)
	{
		pos = SeekNextNotSpace(text, pos);
		if (pos < 0)
			throw new JsonException(EC_ExpectContent, pos, __X("Expect content"));

		WCHAR ch = text[pos];
		if (ch == '{')
			return ParseInnerTextDictionary(node, text, pos);
		else if (ch == '[')
			return ParseInnerTextList(node, text, pos);
		else if (ch == '\"')
			return ParseInnerTextStringValue(node, text, pos);
		else if (ch >= '0' && ch <= '9')
			return ParseInnerTextNumberValue(node, text, pos);
		else if (ch == 'T' || ch == 't' || ch == 'F' || ch == 'f')
			return ParseInnerTextBooleanValue(node, text, pos);
		else if (ch == 'N' || ch == 'n')
			return ParseInnerTextNoneValue(node, text, pos);
		else
			throw new JsonException(EC_UnexpectedSymbol, pos, __X("Unexpected symbol"));
	}

	int ParseInnerTextStringValue(ref<JsonNode> node, stringx text, int pos)
	{
		ASSERT(text[pos] == '\"');

		node->NodeType = JSN_String;
		node->TextValue = ParseStringValueToken(text,  pos);
		return pos;
	}


	int ParseInnerTextNumberValue(ref<JsonNode> node, stringx text, int pos)
	{
		ASSERT(text[pos] >= '0' && text[pos] <= '9');

		bool fFloat = ParseNumberValueToken(text, pos, node->IntValue, node->DblValue);
		node->NodeType = fFloat ? JSN_Float : JSN_Integer;
		return pos;
	}

	int ParseInnerTextBooleanValue(ref<JsonNode> node, stringx text, int pos)
	{
		node->NodeType = JSN_Boolean;
		node->IntValue = ParseBooleanValueToken(text, pos) ? 1 : 0;
		return pos;
	}

	int ParseInnerTextNoneValue(ref<JsonNode> node, stringx text, int pos)
	{
		node->NodeType = JSN_None;
		ParseNoneValueToken(text, pos);
		return pos;
	}


	int ParseInnerTextDictionary(ref<JsonNode> node, stringx text, int pos)
	{
		ASSERT(text[pos] == '{');

		node->NodeType = JSN_Dictionary;
		node->NodeDict = gc_new<JsonNode::JsonNodeDictionary>();
		++pos;
		int flag = 0;	// last symbol: 0 init; 1 comma, 2 content

		while (true)
		{
			pos = SeekNextNotSpace(text, pos);
			if (pos < 0)
				throw new JsonException(EC_Expect, pos, __X("Expect end of dictionary node"));

			WCHAR ch = text[pos];
			if (ch == '\"')
			{
				if (flag == 2)
					throw new JsonException(EC_ExpectSymbol, pos, __X("Expect , symbol"));

				stringx key = ParseStringValueToken(text, pos);
				if (key.Length() == 0)
					throw new JsonException(EC_EmptyArg, pos, __X("Empty key"));

				pos = SeekNextNotSpace(text, pos);
				if (pos < 0 || text[pos] != ':')
					throw new JsonException(EC_Expect, pos, __X("Expect :"));

				ref<JsonNode> subnode = gc_new<JsonNode>();
				subnode->Name = key;

				//--------------------------------------------------------
				//Add function mapping to Insert function
				//--------------------------------------------------------
				node->NodeDict->Insert(key, subnode);

				pos = ParseInnerTextVariant(subnode, text, ++pos);
				flag = 2;
			}
			else if (ch == ',')
			{
				if (flag != 2)
					throw new JsonException(EC_UnexpectedSymbol, pos, __X("Unexpected , symbol"));
				++pos;
				flag = 1;
			}
			else if (ch == '}')
			{
				if (flag == 1)
					throw new JsonException(EC_UnexpectedSymbol, pos, __X("Unexpected } symbol"));
				++pos;
				break;
			}
			else
				throw new JsonException(EC_UnexpectedSymbol, pos, __X("Unexpected symbol"));
		}
		return pos;
	}

	int ParseInnerTextList(ref<JsonNode> node, stringx text, int pos)
	{
		ASSERT(text[pos] == '[');
	
		node->NodeType = JSN_NodeList;
		node->NodeList = gc_new<JsonNode::JsonNodeList>();
		++pos;
		int flag = 0;	// last symbol: 0 init; 1 comma, 2 content

		while (true)
		{
			pos = SeekNextNotSpace(text, pos);
			if (pos < 0)
				throw new JsonException(EC_Expect, pos, __X("Expect end of dictionary node"));

			WCHAR ch = text[pos];
			if (ch == ']')
			{
				if (flag == 1)
					throw new JsonException(EC_UnexpectedSymbol, pos, __X("Unexpected ] symbol"));
				++pos;
				break;
			}
			else if (ch == ',')
			{
				if (flag != 2)
					throw new JsonException(EC_UnexpectedSymbol, pos, __X("Unexpected , symbol"));
				++pos;
				flag = 1;
			}
			else
			{
				if (flag == 2)
					throw new JsonException(EC_ExpectSymbol, pos, __X("Expect , symbol"));

				ref<JsonNode> subnode = gc_new<JsonNode>();
				node->NodeList->Add(subnode);
				pos = ParseInnerTextVariant(subnode, text, pos);
				flag = 2;
			}
		}
		return pos;
	}

	stringx ParseStringValueToken(stringx text, int& pos)
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
		throw new JsonException(EC_Expect, pos, __X("Expect end of value"));
	}
	
	bool ParseNumberValueToken(stringx text, int& pos, int& ivalue, double& fvalue)
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

	int ParseNumberValueIntPart(stringx text, int& pos)
	{
		ASSERT(text[pos] >= '0' && text[pos] <= '9');
		int value = 0;
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

	double ParseNumberValueDecimalPart(stringx text, int& pos)
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

	bool ParseBooleanValueToken(stringx text, int& pos)
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
			throw new JsonException(EC_Unexpected, pos, __X("Unexpected value token"));
	}

	void ParseNoneValueToken(stringx text, int& pos)
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
			throw new JsonException(EC_Unexpected, pos, __X("Unexpected null token"));
	}

	int SeekNextNotSpace(stringx text, int pos)
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

public:
	JsonTextParser(ref<JsonDom> dom);
	void	Parse(stringx text);
};

// -------------------------------------------------------------------------- //

JsonTextParser::JsonTextParser(ref<JsonDom> dom): m_dom(dom)
{
}

void JsonTextParser::Parse(stringx text)
{
	ref<JsonNode> root = m_dom->Root;	// Root node
	int endp = ParseInnerTextVariant(root, text, 0);

	if (endp >= 0 && endp < text.Length())
	{
		endp = SeekNextNotSpace(text, endp);
		if (endp >= 0)
			throw new JsonException(EC_Unexpected, endp, __X("Unexpected content"));
	}	
}

// -------------------------------------------------------------------------- //

void JsonDom::Load(stringx text)
{
	JsonTextParser(this).Parse(text);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
