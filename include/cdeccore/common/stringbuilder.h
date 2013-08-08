#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

class StringBuilder: public Object
{
	DECLARE_REF_CLASS(StringBuilder)

private:
	std::wstring16 m_content;

public:
	StringBuilder() {}
	StringBuilder(int capacity) { m_content.reserve(capacity); }

	int		Length() { return m_content.size(); }
	int		Capacity() { return m_content.capacity(); }

	WCHAR	CharAt(int i) { return m_content[i]; }

	StringBuilder*	Append(WCHAR ch) { m_content.append(1, ch); return this; }
	StringBuilder*	Append(stringx s) { m_content.append(s.c_str(), s.Length()); return this; }

	void	Insert(int pos, WCHAR ch) { m_content.insert(m_content.begin() + pos, ch); }
	void	Insert(int pos, stringx s) { m_content.insert(pos, s.c_str(), s.Length()); }

	void	Remove(int pos, int len);

	std::wstring16 ToStdString() { return m_content; }
	stringx ToString() { return stringx(m_content); }
};

// -------------------------------------------------------------------------- //

inline void StringBuilder::Remove(int pos, int len)
{
	if (pos < 0 || len < 0 || pos > Length() || pos + len > Length())
		cdec_throw(Exception(EC_OutOfRange));

	std::wstring16::iterator it = m_content.begin() + pos;
	m_content.erase(it, it + len);
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
