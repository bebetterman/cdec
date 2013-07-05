#include "stdafx.h"
using namespace  cdec;

// -------------------------------------------------------------------------- //
// Connection
// -------------------------------------------------------------------------- //
Connection::Connection(stringx dburl, stringx uname, stringx pwd, stringx database)
{
	m_driver = get_driver_instance();
	std::string sddburl = Strx2SqlStr(dburl);
	std::cout << sddburl << std::endl;
	m_impl = m_driver->connect(sddburl,Strx2SqlStr(uname),Strx2SqlStr(pwd));
	m_impl->setSchema(Strx2SqlStr(database));
}

void Connection::Close()
{
	try
	{
		m_impl->close();
		delete m_impl;
	} catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "  on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

ref<Statement> Connection::CreateStatement()
{
	sql::Statement *stmt = m_impl->createStatement();
	ref<Statement> s = gc_new<Statement>(stmt);
	return s;
}
ref<PrepareStatement> Connection::prepareStatement(stringx sql)
{
	ref<Encoding>	 encode = Encoding::get_UTF8();
	std::string stdSql = encode->FromUnicode(sql);
	sql::PreparedStatement *  sprstmt = m_impl->prepareStatement(stdSql);
	ref<PrepareStatement> prstmt = gc_new<PrepareStatement>(sprstmt);
	return prstmt;
}