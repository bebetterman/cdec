#include "stdafx.h"
#include "codeconv.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Encoding
// -------------------------------------------------------------------------- //

Encoding::CodePageClass Encoding::get_Class() const
{
	ASSERT(_cp != 0);
	switch (_cp)
	{
	case UTF16:
	case UTF16BE:
		return UTF_WIDE;
		// case UTF7:	// 暂不支持
	case UTF8:
		return UTF_MBCS;
	case 932:
	case 936:
	case 949:
	case 950:
	case 1361:
		return DBCS;
	default:
		return SBCS;
	}
}

stringx Encoding::get_WebName() const
{
	int	n;
	const IanaMapItem* p = GetIanaMap(n);
	for (int i = 0; i < n; ++i, ++p)
	{
		if (p->CP == _cp)
			return p->Name;
	}
	return stringx();
}

ref<Encoding> Encoding::get_Default()
{
	static UINT	cp = PxCodeConv::GetACP();
	return gc_new<Encoding>(cp);
}

const Encoding::IanaMapItem* Encoding::GetIanaMap(int& count)
{
	// 字符集的 IANA 名字，没有找到对应的 API 支持，但是 .NET 库有
	// 因此，把结果整理成了一张表，直接写死在程序中
	// 我看到 xerces-c 里面也是自己建了一个表，参见 XMLTransService::initTransService
	//
	// IANA 官方参考
	// http://www.iana.org/assignments/character-sets
	//
	// .NET 枚举代码
	//
	//	EncodingInfo[] eis = Encoding.GetEncodings();
	//	foreach (EncodingInfo ei in eis)
	//	{
	//		sw.WriteLine("{{ {0},\tL\"{1}\" }},", ei.CodePage, ei.Name);
	//	}

	static IanaMapItem iana_map[] =
	{
		{ 37,	__X("IBM037") },
		{ 437,	__X("IBM437") },
		{ 500,	__X("IBM500") },
		{ 708,	__X("ASMO-708") },
		{ 720,	__X("DOS-720") },
		{ 737,	__X("ibm737") },
		{ 775,	__X("ibm775") },
		{ 850,	__X("ibm850") },
		{ 852,	__X("ibm852") },
		{ 855,	__X("IBM855") },
		{ 857,	__X("ibm857") },
		{ 858,	__X("IBM00858") },
		{ 860,	__X("IBM860") },
		{ 861,	__X("ibm861") },
		{ 862,	__X("DOS-862") },
		{ 863,	__X("IBM863") },
		{ 864,	__X("IBM864") },
		{ 865,	__X("IBM865") },
		{ 866,	__X("cp866") },
		{ 869,	__X("ibm869") },
		{ 870,	__X("IBM870") },
		{ 874,	__X("windows-874") },
		{ 875,	__X("cp875") },
		{ 932,	__X("shift_jis") },
		{ 936,	__X("gb2312") },
		{ 949,	__X("ks_c_5601-1987") },
		{ 950,	__X("big5") },
		{ 1026,	__X("IBM1026") },
		{ 1047,	__X("IBM01047") },
		{ 1140,	__X("IBM01140") },
		{ 1141,	__X("IBM01141") },
		{ 1142,	__X("IBM01142") },
		{ 1143,	__X("IBM01143") },
		{ 1144,	__X("IBM01144") },
		{ 1145,	__X("IBM01145") },
		{ 1146,	__X("IBM01146") },
		{ 1147,	__X("IBM01147") },
		{ 1148,	__X("IBM01148") },
		{ 1149,	__X("IBM01149") },
		{ 1200,	__X("utf-16") },
		{ 1201,	__X("unicodeFFFE") },
		{ 1250,	__X("windows-1250") },
		{ 1251,	__X("windows-1251") },
		{ 1252,	__X("Windows-1252") },
		{ 1253,	__X("windows-1253") },
		{ 1254,	__X("windows-1254") },
		{ 1255,	__X("windows-1255") },
		{ 1256,	__X("windows-1256") },
		{ 1257,	__X("windows-1257") },
		{ 1258,	__X("windows-1258") },
		{ 1361,	__X("Johab") },
		{ 10000,	__X("macintosh") },
		{ 10001,	__X("x-mac-japanese") },
		{ 10002,	__X("x-mac-chinesetrad") },
		{ 10003,	__X("x-mac-korean") },
		{ 10004,	__X("x-mac-arabic") },
		{ 10005,	__X("x-mac-hebrew") },
		{ 10006,	__X("x-mac-greek") },
		{ 10007,	__X("x-mac-cyrillic") },
		{ 10008,	__X("x-mac-chinesesimp") },
		{ 10010,	__X("x-mac-romanian") },
		{ 10017,	__X("x-mac-ukrainian") },
		{ 10021,	__X("x-mac-thai") },
		{ 10029,	__X("x-mac-ce") },
		{ 10079,	__X("x-mac-icelandic") },
		{ 10081,	__X("x-mac-turkish") },
		{ 10082,	__X("x-mac-croatian") },
		{ 12000,	__X("utf-32") },
		{ 12001,	__X("utf-32BE") },
		{ 20000,	__X("x-Chinese-CNS") },
		{ 20001,	__X("x-cp20001") },
		{ 20002,	__X("x-Chinese-Eten") },
		{ 20003,	__X("x-cp20003") },
		{ 20004,	__X("x-cp20004") },
		{ 20005,	__X("x-cp20005") },
		{ 20105,	__X("x-IA5") },
		{ 20106,	__X("x-IA5-German") },
		{ 20107,	__X("x-IA5-Swedish") },
		{ 20108,	__X("x-IA5-Norwegian") },
		{ 20127,	__X("us-ascii") },
		{ 20261,	__X("x-cp20261") },
		{ 20269,	__X("x-cp20269") },
		{ 20273,	__X("IBM273") },
		{ 20277,	__X("IBM277") },
		{ 20278,	__X("IBM278") },
		{ 20280,	__X("IBM280") },
		{ 20284,	__X("IBM284") },
		{ 20285,	__X("IBM285") },
		{ 20290,	__X("IBM290") },
		{ 20297,	__X("IBM297") },
		{ 20420,	__X("IBM420") },
		{ 20423,	__X("IBM423") },
		{ 20424,	__X("IBM424") },
		{ 20833,	__X("x-EBCDIC-KoreanExtended") },
		{ 20838,	__X("IBM-Thai") },
		{ 20866,	__X("koi8-r") },
		{ 20871,	__X("IBM871") },
		{ 20880,	__X("IBM880") },
		{ 20905,	__X("IBM905") },
		{ 20924,	__X("IBM00924") },
		{ 20932,	__X("EUC-JP") },
		{ 20936,	__X("x-cp20936") },
		{ 20949,	__X("x-cp20949") },
		{ 21025,	__X("cp1025") },
		{ 21866,	__X("koi8-u") },
		{ 28591,	__X("iso-8859-1") },
		{ 28592,	__X("iso-8859-2") },
		{ 28593,	__X("iso-8859-3") },
		{ 28594,	__X("iso-8859-4") },
		{ 28595,	__X("iso-8859-5") },
		{ 28596,	__X("iso-8859-6") },
		{ 28597,	__X("iso-8859-7") },
		{ 28598,	__X("iso-8859-8") },
		{ 28599,	__X("iso-8859-9") },
		{ 28603,	__X("iso-8859-13") },
		{ 28605,	__X("iso-8859-15") },
		{ 29001,	__X("x-Europa") },
		{ 38598,	__X("iso-8859-8-i") },
		{ 50220,	__X("iso-2022-jp") },
		{ 50221,	__X("csISO2022JP") },
		{ 50222,	__X("iso-2022-jp") },
		{ 50225,	__X("iso-2022-kr") },
		{ 50227,	__X("x-cp50227") },
		{ 51932,	__X("euc-jp") },
		{ 51936,	__X("EUC-CN") },
		{ 51949,	__X("euc-kr") },
		{ 52936,	__X("hz-gb-2312") },
		{ 54936,	__X("GB18030") },
		{ 57002,	__X("x-iscii-de") },
		{ 57003,	__X("x-iscii-be") },
		{ 57004,	__X("x-iscii-ta") },
		{ 57005,	__X("x-iscii-te") },
		{ 57006,	__X("x-iscii-as") },
		{ 57007,	__X("x-iscii-or") },
		{ 57008,	__X("x-iscii-ka") },
		{ 57009,	__X("x-iscii-ma") },
		{ 57010,	__X("x-iscii-gu") },
		{ 57011,	__X("x-iscii-pa") },
		{ 65000,	__X("utf-7") },
		{ 65001,	__X("utf-8") },

		// 附加的别名，手工增加的
		{ UTF16,	__X("unicode") },
	};
	count = sizeof(iana_map) / sizeof(iana_map[0]);
	return iana_map;
}

ref<Encoding> Encoding::FromWebName(stringx name)
{
	ref<Encoding>	encoding = gc_new<Encoding>();
	int	n;
	const IanaMapItem* p = GetIanaMap(n);
	pin_ptr<WCHAR> pinNameStr = name.ContentBuffer();
	PCWSTR name_cstr = pinNameStr.ptr();

	for (int i = 0; i < n; ++i, ++p)
	{
		// 效率不高
		if (wstricmp16(name_cstr, p->Name) == 0)
		{
			encoding->_cp = p->CP;
			break;
		}
	}
	return encoding;
}

stringx Encoding::ToUnicode(const char* p, size_t cb)
{
	ASSERT(_cp != 0 && !IsWideChar());
	if (cb == 0)
		return stringx(__X(""));

	std::wstring16	ss;
	ss.resize(cb, 0);	// Assume enough space
	// MSDN 对 flags 的说明，部分代码页这里必须为 0
	int convert = PxCodeConv::MultiByteToWideChar(_cp, 0, p, cb, (PWSTR)ss.data(), ss.size());
	if (convert == 0)
		cdec_throw(Exception(EC_Fail));

	ss.resize(convert, 0);
	return stringx(ss);
}

std::string Encoding::FromUnicode(PCWSTR p, UINT cch)
{
	ASSERT(_cp != 0 && !IsWideChar());
	if (cch == 0)
		return std::string();

	std::string		ss;
	ss.resize(cch * 2, 0);	// Estimated space
	// MSDN 对 flags 的说明，使用 0 效率最高
	int convert = PxCodeConv::WideCharToMultiByte(_cp, 0, p, cch, (char*)ss.data(), ss.size());
	if (convert == 0)
	{
	    #if defined(X_OS_WINDOWS)
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			cdec_throw(Exception(EC_OutOfBuffer));
		#endif
		convert = PxCodeConv::WideCharToMultiByte(_cp, 0, p, cch, NULL, 0);
		ASSERT(convert > 0);
		ss.resize(convert, 0);	// Enlarge the buffer to meet demanded size
		convert = PxCodeConv::WideCharToMultiByte(_cp, 0, p, cch, (char*)ss.data(), ss.size());
	}
	ASSERT(convert > 0 && static_cast<size_t>(convert) <= ss.size());
	ss.resize(static_cast<size_t>(convert), 0);
	return ss;
}

bool Encoding::IsDBCSLeading(BYTE ch)
{
	ASSERT(_cp != 0 && get_Class() == DBCS);
	return PxCodeConv::IsDBCSLeadByteEx(_cp, ch) != 0;
}

// 返回 -1 表示给定的 ch 是拖尾字符
// 返回 -2 表示给定的 ch 不合法（1111111x）
int Encoding::GetUtf8TrailingCount(BYTE ch)
{
	if ((ch & 0x80) == 0)
		return 0;	// Standalone, U-00000000 - U-0000007F: 0xxxxxxx
	else if ((ch & 0x40) == 0)
		return -1;	// Traling byte, 10xxxxxx
	else if ((ch & 0x20) == 0)
		return 1;	// U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
	else if ((ch & 0x10) == 0)
		return 2;	// U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
	else if ((ch & 8) == 0)
		return 3;	// U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	else if ((ch & 4) == 0)
		return 4;	// U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	else if ((ch & 2) == 0)
		return 5;	// U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	else
		return -2;
}

std::string Encoding::EncodeUtf8Char(WCHAR ch)
{
	std::string s;
	if ((ch & 0xF800) != 0)
	{
		// U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
		s.append(1, 0xE0 | (ch >> 12));
		s.append(1, 0x80 | ((ch & 0xFFF) >> 6));
		s.append(1, 0x80 | (ch & 0x3F));
	}
	else if ((ch & 0xFF80) != 0)
	{
		// U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
		ASSERT(ch < 0x800);
		s.append(1, 0xC0 | (ch >> 6));
		s.append(1, 0x80 | (ch & 0x3F));
	}
	else
	{
		// Standalone, U-00000000 - U-0000007F: 0xxxxxxx
		ASSERT(ch < 0x80);
		s.append(1, ch);
	}
	return s;
}

WCHAR Encoding::DecodeUtf8Char(const char* ch, int cb)
{
	// 默认设置下 char 是有符号数。转成 WORD 会扩展符号位
	const BYTE* _ch = (const BYTE*)ch;
	int nTrailing = GetUtf8TrailingCount(_ch[0]);
	if (cb != nTrailing + 1)
		cdec_throw(Exception(EC_InvalidValue));
	switch (nTrailing)
	{
	case 0:
		return _ch[0];
	case 1:
		return (((WORD)_ch[0] & 0x1F) << 6) | ((WORD)_ch[1] & 0x3F);
	case 2:
		return (((WORD)_ch[0] & 0xF) << 12) | (((WORD)_ch[1] & 0x3F) << 6) | ((WORD)_ch[2] & 0x3F);
	default:
		cdec_throw(Exception(EC_InvalidValue));
	}
}

Encoding::CharCompletion Encoding::ValidateUtfMultiByteChars(const char* p, int cb)
{
	if (_cp == UTF8)
	{
		int nTrailing = GetUtf8TrailingCount(*p);
		if (nTrailing + 1 == cb)
		{
			for (int i = 0; i < nTrailing; ++i)
				if (!IsUtf8TrailingChar(*++p))
					return CharError;
			return CharCompleted;
		}
		else if (nTrailing + 1 > cb)
			return CharLeading;
		else
			return CharError;
	}
	else
		cdec_throw(Exception(EC_NotImplemented));
}

Encoding::CharCompletion Encoding::ValidateMultiByteChars(const char* p, int cb)
{
	ASSERT(_cp != 0 && !IsWideChar());

	switch (get_Class())
	{
	case SBCS:
		return cb == 1 ? CharCompleted : CharError;
	case DBCS:
		if (IsDBCSLeading(*p))
			return cb == 2 ? CharCompleted : (cb == 1 ? CharLeading : CharError);
		else
			return cb == 1 ? CharCompleted : CharError;
		break;
	case UTF_MBCS:
		return ValidateUtfMultiByteChars(p, cb);
	default:
		cdec_throw(Exception(EC_NotImplemented));
	}
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
