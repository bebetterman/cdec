#include "stdafx.h"
#include "pool.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// DbConnectionManager
// -------------------------------------------------------------------------- //

void ConnectionManager::LoadXmlConfig(stringx path, ConnectionManager::Config& config)
{
	ref<XmlDocument> doc = gc_new<XmlDocument>();
	doc->Load(path);

	ref<XmlElement> eDs = doc->get_DocumentElement();
	if (eDs->get_NodeName() != __X("dbconfig"))
		cdec_throw(Exception(EC_InvalidValue));

	ref<XmlElement> eConfig = eDs->SelectSingleNode(__X("config"));
	if (eConfig == NULL)
		cdec_throw(Exception(EC_InvalidPtr));

	config.Url = eConfig->get_Attribute(__X("dburl"))->get_Value();
	config.Username = eConfig->get_Attribute(__X("uname"))->get_Value();
	config.Password = eConfig->get_Attribute(__X("pwd"))->get_Value();
	config.Database = eConfig->get_Attribute(__X("database"))->get_Value();
}

void ConnectionManager::Init(Config config)
{
	ref<ConnResourceFactory> factory = gc_new<ConnResourceFactory>(config);
	m_pool = gc_new<ResourcePool>(factory);
}

ref<Connection> ConnectionManager::Take()
{
	ref<IResource> rs = m_pool->Take();
	ref<ConnResource> r = ref_cast<ConnResource>(rs);
	return gc_new<Connection>(this, r);
}

ref<Connection> ConnectionManager::TakeCreate()
{
	ref<IResource> rs = m_pool->TakeCreate();
	ref<ConnResource> r = ref_cast<ConnResource>(rs);
	return gc_new<Connection>(this, r);
}

void ConnectionManager::ReturnByAgent(ref<IResource> r)
{
	m_pool->Return(r);
}

void ConnectionManager::Close()
{
	m_pool->Dispose();
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
