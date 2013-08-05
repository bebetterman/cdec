#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class ConnResource: public IResource
{
	DECLARE_REF_CLASS(ConnResource)

protected:
	int	m_index;
	sql::Connection*	m_impl;

public:
	ConnResource(int index, sql::Connection* impl): m_index(index), m_impl(impl)
	{
	}

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
CDEC_NS_END
