#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

void stringx::_Append(PCWSTR pwszRhs, size_t lenRhs)
{
	size_t lenLhs = m_pContent->Length();
	PCWSTR pwszLhs = m_pContent->Content();

	size_t lenResult = lenLhs + lenRhs;
	StringContent* pcontNew = StringContent::CreateRawInstance(lenResult);

	pcontNew->CopyChars(pwszLhs, lenLhs, 0);
	pcontNew->CopyChars(pwszRhs, lenRhs, lenLhs);
	pcontNew->WriteTerminatorChar();

	Clear();
	_Assign(pcontNew);
	pcontNew->Release();
}

bool stringx::StartsWith(stringx subs) const
{
	if (subs.Length() == 0)
		return true;

	if (Length() < subs.Length())
		return false;

	return wstrncmp16(m_pContent->Content(), subs.m_pContent->Content(), subs.Length()) == 0;
}

bool stringx::EndsWith(stringx subs) const
{
	if (subs.Length() == 0)
		return true;

	if (Length() < subs.Length())
		return false;

	size_t index =	Length() -  subs.Length();
	return wstrcmp16(m_pContent->Content() + index, subs.m_pContent->Content()) == 0;
}

stringx stringx::ToUpper() const
{
	size_t len = Length();
	stringx result;
	if (len == 0)
		return result;

	StringContent* pct = StringContent::CreateRawInstance(len);
	WCHAR* pt = pct->Content();
	const WCHAR* ps = c_str();
	for (size_t i = 0; i < len; ++i)
	{
		WCHAR ch = *ps++;
		if (ch >= 'a' && ch <= 'z')
			ch -= 32;
		*pt++ = ch;
	}
	pct->WriteTerminatorChar();

	result._Assign(pct);
	pct->Release();
	return result;
}

stringx stringx::ToLower() const
{
	size_t len = Length();
	stringx result;
	if (len == 0)
		return result;

	StringContent* pct = StringContent::CreateRawInstance(len);
	WCHAR* pt = pct->Content();
	const WCHAR* ps = c_str();
	for (size_t i = 0; i < len; ++i)
	{
		WCHAR ch = *ps++;
		if (ch >= 'A' && ch <= 'Z')
			ch += 32;
		*pt++ = ch;
	}
	pct->WriteTerminatorChar();

	result._Assign(pct);
	pct->Release();
	return result;
}

stringx& stringx::Append(const stringx& s)
{
	if (s == NULL)
		return *this;

	if (*this == NULL)
	{
		*this = s;
		return *this;
	}

	if (s.Length() == 0)
		return *this;

	if (Length() == 0)
	{
		*this = s;
		return *this;
	}

	StringContent* pcontRhs = s.m_pContent;
	_Append(pcontRhs->Content(), pcontRhs->Length());

	return *this;
}

stringx& stringx::Append(PCWSTR p, int count)
{
	if (p == NULL)
		return *this;

	if (*this == NULL)
	{
		*this = p;
		return *this;
	}

	if (*p == 0)
		return *this;

	if (Length() == 0)
	{
		*this = p;
		return *this;
	}

	if (count == -1)
		count = wstrlen16(p);
	_Append(p, count);
	return *this;
}

stringx stringx::PadLeft(WCHAR ch, int n)
{
	int len = Length();
	if (len >= n)
		return *this;

	StringContent* pct = StringContent::CreateRawInstance(n);
	WCHAR* pt = pct->Content();
	for (int i = 0; i < n - len; ++i)
		*pt++ = ch;
	memcpy(pt, c_str(), len + len);
	pct->WriteTerminatorChar();

	stringx result;
	result._Assign(pct);
	pct->Release();
	return result;
}

stringx stringx::PadRight(WCHAR ch, int n)
{
	int len = Length();
	if (len >= n)
		return *this;

	StringContent* pct = StringContent::CreateRawInstance(n);
	WCHAR* pt = pct->Content();
	memcpy(pt, c_str(), len + len);
	pt += len;
	for (int i = len; i < n; ++i)
		*pt++ = ch;
	pct->WriteTerminatorChar();

	stringx result;
	result._Assign(pct);
	pct->Release();
	return result;
}

bool stringx::_Equal(stringx s) const
{
	StringContent* pContent2 = s.m_pContent;

	if (m_pContent == pContent2)
		return true;

	if (m_pContent == NULL || pContent2 == NULL)
		return false;

	if (m_pContent->Length() != pContent2->Length())
		return false;

	return wstrcmp16(m_pContent->Content(), pContent2->Content()) == 0;
}

bool stringx::_Equal(PCWSTR p) const
{
	if (m_pContent == NULL)
		return p == NULL;

	if (p == NULL)
		return false;

	return wstrcmp16(m_pContent->Content(), p) == 0;
}

bool stringx::_Equal(const std::wstring16& s) const
{
	if (m_pContent == NULL)
		return false;

	if (m_pContent->Length() != s.size())
		return false;

	return wstrcmp16(m_pContent->Content(), s.c_str()) == 0;
}

bool stringx::_Greater(stringx s) const
{
	StringContent* pContent2 = s.m_pContent;

	if (m_pContent == NULL)
		return false;

	if (pContent2 == NULL)
		return true;

	return wstrcmp16(m_pContent->Content(), pContent2->Content()) > 0;
}

bool stringx::_Greater(PCWSTR p) const
{
	if (m_pContent == NULL)
		return false;

	if (p == NULL)
		return true;

	return wstrcmp16(m_pContent->Content(), p) > 0;
}

bool stringx::_Greater(const std::wstring16& s) const
{
	if (m_pContent == NULL)
		return false;

	return wstrcmp16(m_pContent->Content(), s.c_str()) > 0;
}

bool stringx::_Less(stringx s) const
{
	StringContent* pContent2 = s.m_pContent;

	if (pContent2 == NULL)
		return false;

	if (m_pContent == NULL)
		return true;

	return wstrcmp16(m_pContent->Content(), pContent2->Content()) < 0;
}

bool stringx::_Less(PCWSTR p) const
{
	if (p == NULL)
		return false;

	if (m_pContent == NULL)
		return true;

	return wstrcmp16(m_pContent->Content(), p) < 0;
}

bool stringx::_Less(const std::wstring16& s) const
{
	if (m_pContent == NULL)
		return true;

	return wstrcmp16(m_pContent->Content(), s.c_str()) < 0;
}

int stringx::IndexOf(WCHAR ch, int pos) const
{
	if (m_pContent == NULL)
		return -1;

	PCWSTR cnt = m_pContent->Content();
	int cbs = m_pContent->Length();
	for (; pos < cbs; ++pos)
		if (cnt[pos] == ch)
			return pos;
	return -1;
}

int stringx::IndexOfAny(const WCHAR chs[], int cch, int pos) const
{
	if (m_pContent == NULL)
		return -1;

	PCWSTR cnt = m_pContent->Content();
	int cbs = m_pContent->Length();
	for (; pos < cbs; ++pos)
	{
		WCHAR ch = cnt[pos];
		for (int j = 0; j < cch; ++j)
			if (chs[j] == ch)
				return pos;
	}
	return -1;
}

int stringx::IndexOfAny(ref<ArrayV<WCHAR> > chs, int pos) const
{
	pin_ptr<WCHAR> pin = chs->GetBuffer();
	return IndexOfAny(pin.ptr(), chs->Count(), pos);
}

int stringx::LastIndexOf(WCHAR ch, int pos) const
{
	if (m_pContent == NULL)
		return -1;

	PCWSTR cnt = m_pContent->Content();
	int cbs = m_pContent->Length();
	if (pos < 0 || pos >= cbs)
		pos = cbs;
	while (pos > 0)
		if (cnt[--pos] == ch)
			return pos;
	return -1;
}

int stringx::LastIndexOfAny(const WCHAR chs[], int cch, int pos) const
{
	if (m_pContent == NULL)
		return -1;

	PCWSTR cnt = m_pContent->Content();
	int cbs = m_pContent->Length();
	if (pos < 0 || pos >= cbs)
		pos = cbs;
	while (pos > 0)
	{
		WCHAR ch = cnt[--pos];
		for (int j = 0; j < cch; ++j)
			if (chs[j] == ch)
				return pos;
	}
	return -1;
}

int stringx::LastIndexOfAny(ref<ArrayV<WCHAR> > chs, int pos) const
{
	pin_ptr<WCHAR> pin = chs->GetBuffer();
	return LastIndexOfAny(pin.ptr(), chs->Count(), pos);
}

stringx stringx::Trim()
{
	if (m_pContent == NULL)
		return *this;

	int i = 0, l = m_pContent->Length();
	PCWSTR p = m_pContent->Content();
	ASSERT(p != NULL);

	for (; i < l; ++i)
	{
		WCHAR ch = p[i];
		if (ch > 32)	// Not space and control chars
			break;
	}
	for (; l > i; --l)
	{
		WCHAR ch = p[l - 1];
		if (ch > 32)
			break;
	}

	if (i == 0 && l == m_pContent->Length())
		return *this;
	else
		return Substring(i, l - i);
}

stringx stringx::TrimLeft()
{
	if (m_pContent == NULL)
		return *this;

	int i = 0, l = m_pContent->Length();
	PCWSTR p = m_pContent->Content();
	ASSERT(p != NULL);

	for (; i < l; ++i)
	{
		WCHAR ch = p[i];
		if (ch > 32)	// Not space and control chars
			break;
	}

	if (i == 0)
		return *this;
	else
		return Substring(i, l - i);
}

stringx stringx::TrimRight()
{
	if (m_pContent == NULL)
		return *this;

	int l = m_pContent->Length();
	PCWSTR p = m_pContent->Content();
	ASSERT(p != NULL);

	for (; l > 0; --l)
	{
		WCHAR ch = p[l - 1];
		if (ch > 32)
			break;
	}

	if (l == m_pContent->Length())
		return *this;
	else
		return Substring(0, l);
}

ref<ArrayListV<stringx> > stringx::Split(WCHAR seperator) const
{
	size_t lastPos = 0;
	size_t length = Length();
	PCWSTR rawContent = c_str();
	ref<ArrayListV<stringx> >  result = gc_new<ArrayListV<stringx> >();

	for (size_t i = 0; i < length; ++i)
	{
		if (rawContent[i] == seperator)
		{
			stringx newStr = stringx(rawContent + lastPos, i - lastPos);
			result->Add(newStr);
			lastPos = i + 1;
		}
	}

	// if (lastPos != length)	// Keep the last empty entry
	{
		stringx newStr = stringx(rawContent + lastPos, length - lastPos);
		result->Add(newStr);
	}

	return result;
}

ref<ArrayListV<stringx> > stringx::Split(stringx separators) const
{
	size_t lastPos = 0;
	size_t length = Length(), lengthSeparators = separators.Length();
	PCWSTR rawContent = c_str();
	PCWSTR rawSeparators = separators.c_str();
	ref<ArrayListV<stringx> >  result = gc_new<ArrayListV<stringx> >();

	for (size_t i = 0; i < length; ++i)
	{
		for (size_t j = 0; j < lengthSeparators; j++)
		{
			if (rawSeparators[j] == rawContent[i])
			{
				stringx newStr = stringx(rawContent + lastPos, i - lastPos);
				result->Add(newStr);
				lastPos = i + 1;
				break;
			}
		}
	}

	// if (lastPos != length)	// Kepp the last empty entry
	{
		stringx newStr = stringx(rawContent + lastPos, length - lastPos);
		result->Add(newStr);
	}

	return result;
}

ref<IEnumerator<WCHAR> > stringx::GetEnumerator() const
{
	return gc_new<StringEnumerator>(*this);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
