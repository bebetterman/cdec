#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class ConnResource: public IResource
{
	DECLARE_REF_CLASS(ConnResource)

protected:
	int	m_index;
	bool m_isDamage;
	sql::Connection*	m_impl;

public:
	ConnResource(int index, sql::Connection* impl): m_index(index), m_impl(impl), m_isDamage(false)
	{
	}
	bool IsDamage(){return m_isDamage;}
	void SetDamage(bool isDamage){ m_isDamage = isDamage; }
	int Index() { return m_index; }
	sql::Connection* Impl() { return m_impl; }
	void Dispose();

	sql::Connection*	_Impl() { return m_impl; }
};

// -------------------------------------------------------------------------- //

class ConnResourceFactory: public IResourceFactory
{
	DECLARE_REF_CLASS(ConnResourceFactory)

protected:
	ConnectionManager::Config	m_config;
	sql::Driver*	m_driver;

public:
	ConnResourceFactory(ConnectionManager::Config config);

	ref<IResource> Make(int index);

	void	Dispose();
};

// -------------------------------------------------------------------------- //

class ResourcePool: public IResourcePool
{
	DECLARE_REF_CLASS(ResourcePool)

protected:
	typedef ArrayList<IResource>	ResourceList;
	typedef std::vector<int>		HoleList;

	ref<CriticalSection>	m_cs;
	ref<IResourceFactory>	m_factory;
	ref<ResourceList>		m_rsc;
	HoleList				m_holes;

#ifdef ENABLE_MYSQL_DEBUG
	std::vector<int>		m_spins;
	int						m_spin;
#endif

public:
	ResourcePool(ref<IResourceFactory> factory);

	ref<IResource>		Take();
	ref<IResource>		TakeCreate();

	void	Return(ref<IResource> r);
	void	Dispose();
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
