#include "stdafx.h"
#include "pool.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

void ConnResource::Dispose()
{
	// Refer http://hi.baidu.com/netpet/item/51a8a37806488011d1dcb325

	DESTORY_MYSQL_OBJECT(m_impl, Connection);
}

// -------------------------------------------------------------------------- //

ConnResourceFactory::ConnResourceFactory(ConnectionManager::Config config)
{
	m_config = config;
	m_driver = get_driver_instance();
}

ref<IResource> ConnResourceFactory::Make(int index)
{
	sql::SQLString url_s = Strx2SqlStr(m_config.Url);
	sql::SQLString user_s = Strx2SqlStr(m_config.Username);
	sql::SQLString pass_s = Strx2SqlStr(m_config.Password);
	sql::Connection* impl = m_driver->connect(url_s, user_s, pass_s);
	
	if (m_config.Database != NULL)
	{
		sql::SQLString name_s = Strx2SqlStr(m_config.Database);
		impl->setSchema(name_s);
	}

	return gc_new<ConnResource>(index, impl);
}

void ConnResourceFactory::Dispose()
{
}

// -------------------------------------------------------------------------- //

ResourcePool::ResourcePool(ref<IResourceFactory> factory)
{
	m_cs = gc_new<CriticalSection>();
	m_factory = factory;
	m_rsc = gc_new<ResourceList>();

#ifdef ENABLE_MYSQL_DEBUG
	m_spin = 0;
#endif
}

ref<IResource> ResourcePool::Take()
{
	ref<IResource>	r;
	m_cs->Enter();

	int index;
	if (!m_holes.empty())
	{
		// Obtain a free one
		index = m_holes.back();
		m_holes.pop_back();
		r = m_rsc->at(index);
	}
	else
	{
		// Create a new instance
		index = m_rsc->Count();
		r = m_factory->Make(index);
		m_rsc->Add(r);

#ifdef ENABLE_MYSQL_DEBUG
		MYSQL_ASSERT(m_spins.size() == index);
		m_spins.push_back(0);
#endif
	}

#ifdef ENABLE_MYSQL_DEBUG	
	MYSQL_ASSERT(m_spins[index] == 0);
	m_spins[index] = ++m_spin;
	printf("[POOL] Take at %d, Spin = %d\n", index, m_spin);
#endif

	m_cs->Exit();
	return r;
}

ref<IResource> ResourcePool::TakeCreate()
{
	ref<IResource> r = m_factory->Make(-2);
	return r;
}

void ResourcePool::Return(ref<IResource> r)
{
	int index = r->Index();
	if (index == -2)
	{
		r->Dispose();
		return;
	}
	if(r->IsDamage())
	{
		r->Dispose();
		return;
	}

	ASSERT(index >= 0);
	m_cs->Enter();

#ifdef ENABLE_MYSQL_DEBUG
	MYSQL_ASSERT(m_spins[index] != 0);
	printf("[POOL] Return at %d, Spin = %d\n", index, m_spins[index]);
	m_spins[index] = 0;
#endif

	m_holes.push_back(index);
	m_cs->Exit();
}

void ResourcePool::Dispose()
{
	foreach (ref<IResource>, r, m_rsc)
		r->Dispose();
	m_rsc->Clear();
	m_rsc = NULL;
	m_cs = NULL;
	m_holes.clear();
	m_factory->Dispose();
	m_factory = NULL;
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
