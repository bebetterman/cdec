#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class MYSQLCDECEXPORT PrepareStatement: public Object
{
	DECLARE_REF_CLASS(PrepareStatement)

protected:
	 sql::PreparedStatement* m_impl;

public:
	PrepareStatement(sql::PreparedStatement* pstmt): m_impl(pstmt) {}

	void	SetString(int index, stringx value);
	void	SetInt(int index, int value);
	void	SetInt64(int index, INT64 value);
	
	int		ExecuteUpdate();
	ref<ResultSet> ExecuteQuery();

	void	Close();
	~PrepareStatement() { Close(); }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
