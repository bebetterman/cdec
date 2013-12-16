// -------------------------------------------------------------------------- //
//	XMLCE - SAX Reader
//	Created 2008-4-10
//	Updated 2012-7-31 Adapted for c-- style
// -------------------------------------------------------------------------- //

#include "stdafx.h"
#include "parsecore.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

void XmlReader::VerifyEncoding(ref<StreamReader> tr)
{
	WCHAR ch = 0;
	while (!tr->IsEnd())
	{
		ch = tr->ReadChar();
		if (!ParserMicroCore::IsSpace(ch))
			break;
	}
	
	if (tr->IsEnd() || ch != '<')
		return;
	
	ch = tr->ReadChar();
	if (ch != '?')
		return;

	std::wstring16 s;
	s.reserve(32);
	do
	{
		s += (ch = tr->ReadChar());
	} while(ch != '>');

	size_t pos = s.find(__X("encoding"));
	if (pos == std::wstring16::npos)
		return;

	pos = s.find_first_of('\"', pos + 8);
	if (pos == std::wstring16::npos)
		throw LXS_AV_BQ;

	size_t pos2 = s.find_first_of('\"', pos + 1);
	if (pos2 == std::wstring16::npos)
		throw LXS_LT;

	s = s.substr(pos + 1, pos2 - pos - 1);
	ref<Encoding> eDetect = tr->get_Encoding();
	ref<Encoding> eSigned = Encoding::FromWebName((stringx)s);
	if (!eSigned->IsValid())
		cdec_throw(Exception(EC_InvalidArg));

	if (eDetect->IsWideChar() || eDetect->get_CP() == Encoding::UTF8)
	{
		if (!eSigned->IsEqual(eDetect))
			cdec_throw(Exception(EC_InvalidArg));
	}
	else
	{
		ASSERT(eDetect->IsEqual(Encoding::get_Default()));
		if (!eSigned->IsWideChar())
			tr->ChangeEncoding(eSigned);
		else
			cdec_throw(Exception(EC_InvalidArg));
	}
}

void XmlReader::ParseText(ref<ISaxAccepter> pAccpt, stringx xmlText)
{
	ParserMicroCore	core;
	core.Reset(pAccpt);
	foreach (WCHAR, ch, xmlText)
	{
		core.SendChar(ch);
	}
	core.Finalize();
}

void XmlReader::Parse(ref<ISaxAccepter> accpt, ref<TextReader> tr)
{
	ParserMicroCore core;
	core.Reset(accpt);
	tr->Reset();
	while (!tr->IsEnd())
		core.SendChar(tr->ReadChar());
	tr->Close();
	core.Finalize();
}

void XmlReader::Parse(ref<ISaxAccepter> accpt, ref<Stream> stream)
{
	ref<StreamReader> tr = gc_new<StreamReader>(stream);
	VerifyEncoding(tr);
	Parse(accpt, ref<TextReader>(tr));
}

void XmlReader::Parse(ref<ISaxAccepter> accpt, stringx filename)
{
	ref<StreamReader> tr = gc_new<StreamReader>(filename);
	VerifyEncoding(tr);
	Parse(accpt, ref<TextReader>(tr));
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
