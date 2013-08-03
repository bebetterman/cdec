#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// DbConnectionManager
// -------------------------------------------------------------------------- //

class MYSQLCDECEXPORT ConnectionManager: public Object
{
	DECLARE_REF_CLASS(DbConnectionManager)

public:
	struct MYSQLCDECEXPORT Config
	{
		stringx	Url;
		stringx	Username, Password;
		stringx Database;
	};

protected:
	Config	m_config;

public:
	ConnectionManager(Config config): m_config(config) {}
	ConnectionManager(stringx path) { LoadXmlConfig(path, m_config); }

	ref<Connection> Take();
	void	Return(ref<Connection> conn) { conn->Close(); }

	void	Close() {}

	static void	LoadXmlConfig(stringx path, Config& config);
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
