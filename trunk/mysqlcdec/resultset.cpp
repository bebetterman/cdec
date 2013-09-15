#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

bool ResultSet::Next()
{
	return m_impl->next();
}

int ResultSet::RowsCount() 
{
	return m_impl->rowsCount();
}

bool ResultSet::IsNull(int index) { return m_impl->isNull(index); }

bool ResultSet::IsNull(stringx key)
{
	sql::SQLString key_s = Strx2SqlStr(key);
	return m_impl->isNull(key_s);
}

int ResultSet::GetInt(int index) { return m_impl->getInt(index); }

int ResultSet::GetInt(stringx key)
{
	sql::SQLString key_s = Strx2SqlStr(key);
	return m_impl->getInt(key_s);
}

UINT ResultSet::GetUInt(int index) { return m_impl->getUInt(index); }

UINT ResultSet::GetUInt(stringx key)
{
	sql::SQLString key_s = Strx2SqlStr(key);
	return m_impl->getUInt(key_s);
}

INT64 ResultSet::GetInt64(int index) { return m_impl->getInt64(index); }

INT64 ResultSet::GetInt64(stringx key)
{
	sql::SQLString key_s = Strx2SqlStr(key);
	return m_impl->getInt64(key_s);
}

UINT64 ResultSet::GetUInt64(int index) { return m_impl->getUInt64(index); }

UINT64 ResultSet::GetUInt64(stringx key)
{
	sql::SQLString key_s = Strx2SqlStr(key);
	return m_impl->getUInt64(key_s);
}

stringx ResultSet::GetString(int index)
{
	sql::SQLString value = m_impl->getString(index);
	return SqlStr2Strx(value);
}

stringx ResultSet::GetString(stringx key)
{
	sql::SQLString key_s = Strx2SqlStr(key);
	sql::SQLString value = m_impl->getString(key_s);
	return SqlStr2Strx(value);
}

void ResultSet::Close()
{
	DESTORY_MYSQL_OBJECT(m_impl, ResultSet);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
