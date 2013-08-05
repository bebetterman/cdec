#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Resource Pool
// -------------------------------------------------------------------------- //

interface IResource: public Object
{
	DECLARE_REF_CLASS(IResource)

	// For debugging
#ifdef _DEBUG
	int	__Used;
	IResource(): __Used(0) {}
#endif

	virtual int Index() = 0;
	virtual void Dispose() = 0;
};

interface IResourceFactory: public Object
{
	DECLARE_REF_CLASS(IResourceFactory)

	virtual ref<IResource> Make(int index) = 0;
	virtual void Dispose() = 0;
};

class ResourcePool: public Object
{
	DECLARE_REF_CLASS(ResourcePool)

protected:
	typedef ArrayList<IResource>	ResourceList;
	typedef std::vector<int>		HoleList;

	ref<CriticalSection>	m_cs;
	ref<IResourceFactory>	m_factory;
	ref<ResourceList>		m_rsc;
	HoleList				m_holes;

public:
	ResourcePool(ref<IResourceFactory> factory);

	ref<IResource>		Take();
	void	Return(int index);
	void	Dispose();
};

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
	ref<ResourcePool>		m_pool;

public:
	ConnectionManager(Config config) { Init(config); }
	ConnectionManager(stringx path);

	ref<Connection> Take();
	void	ReturnByAgent(int index);	// Only to be called by Connection

	// For backward capability
	void	Return(ref<Connection> conn)
	{
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
