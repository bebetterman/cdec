#include "stdafx.h"
#include "pool.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

ResourcePool::ResourcePool(ref<IResourceFactory> factory)
{
	m_cs = gc_new<CriticalSection>();
	m_factory = factory;
	m_rsc = gc_new<ResourceList>();
}

ref<IResource> ResourcePool::Take()
{
	ref<IResource>	r;
	m_cs->Enter();

	if (!m_holes.empty())
	{
		// Obtain a free one
		int index = m_holes.back();
		m_holes.pop_back();
		r = m_rsc->at(index);
	}
	else
	{
		// Create a new instance
		int n = m_rsc->Count();
		r = m_factory->Make(n);
		m_rsc->Add(r);
	}

#ifdef _DEBUG
	ASSERT(r->__Used == 0);
	r->__Used = 1;
	printf("[POOL] Take at %d\n", r->Index());
#endif

	m_cs->Exit();
	return r;
}

void ResourcePool::Return(int index)
{
	m_cs->Enter();

#ifdef _DEBUG
	printf("[POOL] Return at %d\n", index);
	ref<IResource> r = m_rsc->at(index);
	ASSERT(r->__Used != 0);
	r->__Used = 0;
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

void ConnResource::Dispose()
{
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
CDEC_NS_END
