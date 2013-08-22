#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// ConnectionManager
// -------------------------------------------------------------------------- //

class MYSQLCDECEXPORT ConnectionManager : public Object
{
	DECLARE_REF_CLASS(ConnectionManager)

public:
	struct MYSQLCDECEXPORT Config
	{
		stringx	Url;
		stringx	Username, Password;
		stringx Database;
	};

protected:
	ref<IResourcePool>		m_pool;

public:
	ConnectionManager(Config config) { Init(config); }
	ConnectionManager(stringx path);

	ref<Connection> Take();
	ref<Connection>	TakeCreate();
	void	ReturnByAgent(ref<IResource> rc);	// Only to be called by Connection

	// For backward capability
	void	Return(ref<Connection> conn)
	{
		conn->Rollback();
		conn->SetAutoCommit(true);
		conn->Return();
	}

	void	Close();

	static void	LoadXmlConfig(stringx path, Config& config);

protected:
	void	Init(Config config);
};

// -------------------------------------------------------------------------- //

inline ConnectionManager::ConnectionManager(stringx path)
{
	Config config;
	LoadXmlConfig(path, config);
	Init(config);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
