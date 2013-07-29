// This header contains the following:
//   - stringx

#pragma once
#include <cctype>

CDEC_NS_BEGIN

// Predefinitions
template<class _Ty> class ArrayV;
template<class _Ty> class ArrayListV;

// -------------------------------------------------------------------------- //
// Class StringContent
//
// A StringContent holds a shared character array that represents the string
// value.
// Although a NULL string is supported by Class stringx, a StringContent will
// always have a character array. A NULL string is implemented as a stringx
// with no StringContent instance.
// StringContent can only be accessed by stringx.
// -------------------------------------------------------------------------- //

class StringContent
{
	_CDEC_SHARED_CLASS(StringContent)

protected:
	PWSTR		m_pContent;				// Content of string
	size_t		m_length;				// Length (number of characters)
										// Must be 0 if Content is NULL

public:
	size_t		Length() const { return m_length; }
	PWSTR		Content() const { return m_pContent; }

	static StringContent* CreateRawInstance(size_t length)
	{
		StringContent* pThis = _gcCreation::AllocInstanceMemory();
		new(pThis) StringContent(length);
		return pThis;
	}

	static StringContent* CreateInstanceFromString(PCWSTR pSource, size_t length)
	{
		StringContent* pThis = CreateRawInstance(length);
		pThis->CopyChars(pSource, length, 0);
		pThis->WriteTerminatorChar();
		return pThis;
	}

	void CopyChars(PCWSTR pSource, size_t lenSource, size_t offset)
	{
		ASSERT((int)offset >= 0 && (int)lenSource >= 0 && offset + lenSource <= m_length);
		memcpy(m_pContent + offset, pSource, ContentLength(lenSource));
	}

	void WriteTerminatorChar() { m_pContent[m_length] = 0; }

protected:
	// This constructor only allocates memory for content buffer but does not
	// initialize the buffer
	explicit StringContent(size_t length): m_length(length)
	{
		m_pContent = (PWSTR)CoreAllocT(BufferLength(length));
	}
	~StringContent() { Clear(); }

	static size_t ContentLength(size_t length) { return length * sizeof(WCHAR); }
	static size_t BufferLength(size_t length) { return (length + 1) * sizeof(WCHAR); }

	void Clear()
	{
		CoreFreeT(m_pContent, BufferLength(m_length));
		m_pContent = NULL;
		m_length = 0;
	}
};

// -------------------------------------------------------------------------- //
// Class stringx
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT stringx
{
protected:
	StringContent*	m_pContent;

	friend class StringEnumerator;

public:
	// Constructors and destructors
	stringx(): m_pContent(NULL) {}
	stringx(const stringx& r): m_pContent(NULL) { _Assign(r.m_pContent); }
	stringx(PCWSTR pwszText): m_pContent(NULL) { _Assign(pwszText); }
	stringx(PCWSTR pwszText, int count): m_pContent(NULL) { _Assign(pwszText, count); }
	stringx(WCHAR ch, int count): m_pContent(NULL) { _Assign(std::wstring16(count, ch)); }
	explicit stringx(const std::wstring16& s): m_pContent(NULL) { _Assign(s); }
	~stringx() { Clear(); }

	// Assignments
	stringx& operator=(const stringx& r)
	{
		if (&r != this) { Clear(); _Assign(r.m_pContent); }
		return *this;
	}
	stringx& operator=(PCWSTR pwszText) { Clear(); _Assign(pwszText); return *this; }
	stringx& operator=(const std::wstring16& s) { Clear(); _Assign(s); return *this; }

	// Attributes
	size_t Length() const { return m_pContent != NULL ? m_pContent->Length() : 0; }

	// Compare
	bool StartsWith(stringx subs);
	bool EndsWith(stringx subs);

	stringx Substring(unsigned int pos) { ASSERT(pos <= Length()); return Substring(pos, Length() - pos); }
	stringx Substring(unsigned int pos, unsigned int len)
	{
		ASSERT(pos <= Length() && pos + len <= Length());
		return stringx(c_str() + pos, len);
	}

	stringx	ToLower();
	stringx ToUpper();

	int		IndexOf(WCHAR ch, int pos = 0);
	int		IndexOfAny(ref<ArrayV<WCHAR> > chs, int pos = 0);
	int		IndexOfAny(const WCHAR chs[], int cch, int pos = 0);

	int		LastIndexOf(WCHAR ch, int pos = -1);
	int		LastIndexOfAny(ref<ArrayV<WCHAR> > chs, int pos = -1);
	int		LastIndexOfAny(const WCHAR chs[], int cch, int pos = -1);

	stringx	Trim();
	stringx	TrimLeft();
	stringx	TrimRight();

	ref<ArrayListV<stringx> > Split(WCHAR separator);
	ref<ArrayListV<stringx> > Split(stringx separators);

	stringx& Append(const stringx&);
	stringx& Append(PCWSTR, int);
	stringx& Append(PCWSTR p) { return Append(p, -1); }
	stringx& Append(WCHAR ch) { return Append(&ch, 1); }
	
	stringx		PadLeft(WCHAR ch, int n);
	stringx		PadRight(WCHAR ch, int n);

	stringx& operator+=(const stringx& s) { return Append(s); }
	stringx& operator+=(PCWSTR p) { return Append(p); }
	stringx& operator+=(WCHAR ch) { return Append(ch); }

	stringx	operator+(const stringx& s) const
	{
		stringx r = *this;
		return r += s;
	}

	stringx operator+(PCWSTR p) const
	{
		stringx r = *this;
		return r += p;
	}

	stringx operator+(WCHAR ch) const
	{
		stringx r = *this;
		return r += ch;
	}

	bool operator==(const stringx& rhs) const { return _Equal(rhs); }
	bool operator==(PCWSTR rhs) const { return _Equal(rhs); }
	bool operator==(const std::wstring16& rhs) const { return _Equal(rhs); }

	bool operator>(const stringx& rhs) const { return _Greater(rhs); }
	bool operator>(PCWSTR rhs) const { return _Greater(rhs); }
	bool operator>(const std::wstring16& rhs) const { return _Greater(rhs); }

	bool operator<(const stringx& rhs) const { return _Less(rhs); }
	bool operator<(PCWSTR rhs) const { return _Less(rhs); }
	bool operator<(const std::wstring16& rhs) const { return _Less(rhs); }

	bool operator!=(const stringx& rhs) const { return !_Equal(rhs); }
	bool operator!=(PCWSTR rhs) const { return !_Equal(rhs); }
	bool operator!=(const std::wstring16& rhs) const { return !_Equal(rhs); }

	bool operator>=(const stringx& rhs) const { return !_Less(rhs); }
	bool operator>=(PCWSTR rhs) const { return !_Less(rhs); }
	bool operator>=(const std::wstring16& rhs) const { return !_Less(rhs); }

	bool operator<=(const stringx& rhs) const { return !_Greater(rhs); }
	bool operator<=(PCWSTR rhs) const { return !_Greater(rhs); }
	bool operator<=(const std::wstring16& rhs) const { return !_Greater(rhs); }

	// Interoperability
	pin_ptr<WCHAR> ContentBuffer()
	{
		PWSTR p = m_pContent != NULL ? m_pContent->Content() : NULL;
		return pin_ptr<WCHAR>::CreatePinptr(p);
	}

	// This pointer returned is unsafe for no pin_ptr instance are guarding.
	// So it should only be used instantly.
	PCWSTR c_str() const { return m_pContent != NULL ? m_pContent->Content() : NULL; }

	WCHAR operator[](size_t index) const
	{
		return m_pContent->Content()[index];
	}

	std::wstring16 ToStdString() const
	{
		if (m_pContent == NULL)
			return std::wstring16();
		return std::wstring16(m_pContent->Content(), m_pContent->Length());
	}

	// Clear
	void Clear() { if (m_pContent != NULL) { m_pContent->Release(); m_pContent = NULL; } }

	// Enumerator
	ref<IEnumerator<WCHAR> > GetEnumerator();

protected:
	// Assignment
	void _Assign(PCWSTR pwszText)
	{
		ASSERT(m_pContent == NULL);
		size_t length = pwszText != NULL ? wstrlen16(pwszText) : 0;
		_Assign(pwszText, length);
	}
	void _Assign(PCWSTR pwszText, size_t length)
	{
		ASSERT(m_pContent == NULL);
		if (pwszText != NULL)
			m_pContent = StringContent::CreateInstanceFromString(pwszText, length);
	}
	void _Assign(const std::wstring16& s) { _Assign(s.data(), s.length()); }
	void _Assign(StringContent* pContent)
	{
		ASSERT(m_pContent == NULL);
		m_pContent = pContent;
		if (pContent != NULL)
			pContent->AddRef();
	}

	// Appending
	void _Append(PCWSTR pwszText, size_t length);
	void _Append(PCWSTR pwszText)
	{
		if (pwszText != NULL)
			_Append(pwszText, wstrlen16(pwszText));
	}

	// Comparision
	bool _Equal(stringx rhs) const;
	bool _Equal(PCWSTR p) const;
	bool _Equal(const std::wstring16& s) const;
	bool _Greater(stringx rhs) const;
	bool _Greater(PCWSTR p) const;
	bool _Greater(const std::wstring16& s) const;
	bool _Less(stringx rhs) const;
	bool _Less(PCWSTR p) const;
	bool _Less(const std::wstring16& s) const;
};

// -------------------------------------------------------------------------- //
// Class StringEnumerator
// -------------------------------------------------------------------------- //

class StringEnumerator: public _ContinuousEnum<WCHAR>
{
	DECLARE_REF_CLASS(StringEnumerator)

public:
	StringEnumerator(stringx s)
	{
		StringContent* pContent = s.m_pContent;
		if (pContent != NULL)
		{
			m_n = pContent->Length();
			m_p = pContent->Content();
		}
	}
};

// -------------------------------------------------------------------------- //
// Global operators
// -------------------------------------------------------------------------- //

inline stringx operator+(PCWSTR lhs, stringx rhs)
{
	return stringx(lhs) += rhs;
}

inline stringx operator+(WCHAR ch, stringx rhs)
{
	return stringx(&ch, 1) += rhs;
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
