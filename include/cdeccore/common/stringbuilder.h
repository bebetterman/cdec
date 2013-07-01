#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

class CDECCOREEXPORT StringBuilder: public Object
{
	DECLARE_REF_CLASS(StringBuilder)

private:
	std::wstring16 m_content;

public:
	StringBuilder() {}
	StringBuilder(int capacity) { m_content.reserve(capacity); }

	int Length() { return m_content.size(); }
	int Capacity() { return m_content.capacity(); }

	void Append(WCHAR ch) { m_content.append(1, ch); }
	void Append(stringx s) { m_content.append(s.c_str(), s.Length()); }

	void Insert(int pos, WCHAR ch) { m_content.insert(m_content.begin() + pos, ch); }
	void Insert(int pos, stringx s) { m_content.insert(pos, s.c_str(), s.Length()); }

	std::wstring16 ToStdString() { return m_content; }
	stringx ToString() { return stringx(m_content); }
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
