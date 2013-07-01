#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Encoding
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT Encoding : public Object
{
	DECLARE_REF_CLASS(Encoding)

public:
	enum CodePage
	{
		UTF16				= 1200,		// Unicode (UTF-16)
		UTF16BE				= 1201,		// Unicode (UTF-16) Big Endian
		UTF32				= 12000,	// Unicode (UTF-32)
		UTF32BE				= 12001,	// Unicode (UTF-32) Big Endian
		UTF7				= 65000,	// Unicode (UTF-7)
		UTF8				= 65001,	// Unicode (UTF-8)
	};

	enum CodePageClass
	{
		SBCS,							// ���ֽ��ַ�
		DBCS,							// ���ֽ���˫�ֽ��ַ����
		UTF_MBCS,						// UTF-8
		UTF_WIDE,						// UTF-16
	};

	enum CharCompletion
	{
		CharCompleted,		// �������ַ�
		CharLeading,		// �ַ���������Ӧ���к����ַ�
		CharError,			// �ַ��������������Ǵ���
	};

protected:
	struct IanaMapItem
	{
		UINT	CP;
		PCWSTR	Name;
	};

	UINT	_cp;

public:
	Encoding(): _cp(0) {}	// ��Ч�� CP
	Encoding(UINT cp) { ChangeCP(cp); }

	bool	IsValid() const { return _cp != 0; }
	bool	IsWideChar() const { return _cp == UTF16 || _cp == UTF16BE; }
	UINT	get_CP() const { return _cp; }
	CodePageClass get_Class() const;
	// ����ڱ����޷����ҵ���Ӧ��Ŀ���򷵻ؿ��ַ���
	stringx get_WebName() const;
	void	ChangeCP(UINT cp)
	{
		// ��֧�ֵĴ���ҳ
		ASSERT(cp != 0);
		ASSERT(cp != UTF32 && cp != UTF32BE && cp != UTF7);
		_cp = cp;
	}

	stringx			ToUnicode(const char* p, size_t cch);
	stringx			ToUnicode(const std::string& s)
	{
		return ToUnicode(s.data(), s.size());
	}
	stringx			ToUnicode(ref<ArrayV<char> > arrChar, size_t cch)
	{
		pin_ptr<char> pinChar = arrChar->GetBuffer();
		return ToUnicode(pinChar.ptr(), cch);
	}

	std::string		FromUnicode(PCWSTR p, UINT cch);
	std::string		FromUnicode(const std::wstring16& s) { return FromUnicode(s.data(), s.size());	}
	std::string		FromUnicode(stringx s) { return FromUnicode(s.c_str(), s.Length());	}

	static ref<Encoding> get_Default();
	static ref<Encoding> get_Unicode() { return gc_new<Encoding>(UTF16); }
	static ref<Encoding> get_UTF8() { return gc_new<Encoding>(UTF8); }

	bool IsEqual(ref<Encoding> r) const { return _cp == r->_cp; }

	bool IsDBCSLeading(BYTE ch);
	CharCompletion ValidateMultiByteChars(const char* p, int cb);

	static int  GetUtf8TrailingCount(BYTE ch);
	static bool IsUtf8TrailingChar(BYTE ch) { return (ch & 0xC0) == 0x80; }
	static std::string EncodeUtf8Char(WCHAR ch);
	static WCHAR DecodeUtf8Char(const char* ch, int cb);

	// ���ݴ���ҳ�� IANA ����ָ���� Encoding ʵ��
	// �����Ӧ�� IANA ��֧�֣���õ��� Encoding �� IsValid �������� false
	static ref<Encoding> FromWebName(stringx);

protected:
	CharCompletion ValidateUtfMultiByteChars(const char* p, int cb);
	static const IanaMapItem* GetIanaMap(int& count);
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
