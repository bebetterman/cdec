#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class MYSQLCDECEXPORT PreparedStatement: public Object
{
	DECLARE_REF_CLASS(PreparedStatement)

protected:
	 sql::PreparedStatement* m_impl;

public:
	PreparedStatement(sql::PreparedStatement* pstmt): m_impl(pstmt) {}

	void	SetString(int index, stringx value);
	void	SetInt(int index, int value);
	void	SetUInt(int index, UINT value);
	void	SetInt64(int index, INT64 value);
	void	SetUInt64(int index, UINT64 value);
	
	bool	Execute();
	int		ExecuteUpdate();
	ref<ResultSet> ExecuteQuery();

	void	Close();
	~PreparedStatement() { Close(); }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
