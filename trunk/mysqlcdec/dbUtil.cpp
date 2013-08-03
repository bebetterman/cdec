#include "stdafx.h"

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

ref<Connection> ConnectionManager::Take()
{
	ref<Connection> conn = gc_new<Connection>();
	conn->Connect(m_config.Url, m_config.Username, m_config.Password);
	conn->SetSchema(m_config.Database);
	return conn;
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
