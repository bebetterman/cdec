#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Connection
// -------------------------------------------------------------------------- //

class ConnectionManager;

class MYSQLCDECEXPORT Connection : public Object
{
	DECLARE_REF_CLASS(Connection)

protected:
	ref<ConnectionManager>	m_manager;
	int		m_index;
	sql::Connection*	m_impl;

protected:
	Connection(ref<ConnectionManager> manager, int index, sql::Connection* impl);

public:
	int		Index() { return m_index; }

	ref<Statement>			CreateStatement();
	ref<PrepareStatement>	CreatePrepareStatement(stringx sql);

	// Easy operations
	bool	Execute(stringx sql);
	int		ExecuteUpdate(stringx sql);
	ref<ResultSet>	ExecuteQuery(stringx sql);

	void Return();
	~Connection() { Return(); }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
