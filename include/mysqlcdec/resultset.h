#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class MYSQLCDECEXPORT ResultSet : public Object
{
	DECLARE_REF_CLASS(ResultSet)

protected:
	sql::ResultSet *m_impl;

public:
	ResultSet(sql::ResultSet* impl): m_impl(impl) { }
	
	bool	Next();
	int		RowsCount();

	bool	IsNull(int index);
	bool	IsNull(stringx key);

	int		GetInt(int index);
	int		GetInt(stringx key);

	UINT	GetUInt(int index);
	UINT	GetUInt(stringx key);

	INT64	GetInt64(int index);
	INT64	GetInt64(stringx key);

	UINT64	GetUInt64(int index);
	UINT64	GetUInt64(stringx key);

	stringx	GetString(int index);
	stringx	GetString(stringx key);

	void	Close();
	~ResultSet() { Close(); }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
