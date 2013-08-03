#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class MYSQLCDECEXPORT Statement : public Object
{
	DECLARE_REF_CLASS(Statement)

protected:
	sql::Statement *m_impl;

public:
	Statement(sql::Statement* impl): m_impl(impl) { }

    bool	Execute(stringx sql);
    int		ExecuteUpdate(stringx sql);
    ref<ResultSet>	ExecuteQuery(stringx sql);

	void	Close();
	~Statement() { Close(); }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
