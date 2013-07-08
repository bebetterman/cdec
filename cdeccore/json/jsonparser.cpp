#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class JsonTextParser
{
protected:
	ref<JsonDom>	m_dom;

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
}

// -------------------------------------------------------------------------- //

void JsonDom::Load(stringx text)
{
	JsonTextParser(this);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
