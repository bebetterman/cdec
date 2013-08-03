#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class MYSQLCDECEXPORT Connection : public Object
{
	DECLARE_REF_CLASS(Connection)

protected:
	sql::Driver*		m_driver;
	sql::Connection*	m_impl;

public:
	Connection();

	void Connect(stringx url, stringx user, stringx pass);
	void SetSchema(stringx database);

	ref<Statement>			CreateStatement();
	ref<PrepareStatement>	CreatePrepareStatement(stringx sql);

	// Easy operations
	bool	Execute(stringx sql);
	int		ExecuteUpdate(stringx sql);
	ref<ResultSet>	ExecuteQuery(stringx sql);

	void Close();
	~Connection() { Close(); }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
