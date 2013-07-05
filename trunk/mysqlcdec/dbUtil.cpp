#include "stdafx.h"
using namespace  cdec;


/*convert sql::SQLString to cdec::stringx*/
stringx SqlStr2Strx(sql::SQLString sqlStr, ref<Encoding> encode)
{
	return encode->ToUnicode(sqlStr.asStdString());
}

/*convert cdec::stringx to sql::SQLString*/
sql::SQLString Strx2SqlStr(stringx strx, ref<Encoding> encode)
{
    std::string stdSql = encode->FromUnicode(strx);
    return sql::SQLString(stdSql);
}

// -------------------------------------------------------------------------- //
// DbUtil
// -------------------------------------------------------------------------- //
DbUtil::DbUtil(stringx pathConfig)
{
	ref<XmlDocument> doc = gc_new<XmlDocument>();
	doc->Load(pathConfig);

	ref<XmlElement> eDs = doc->get_DocumentElement();
	if (eDs->get_NodeName() != __X("dbconfig"))
		cdec_throw(Exception(EC_InvalidValue));

	ref<XmlElement> eConfig = eDs->SelectSingleNode(__X("config"));
	if (eConfig == NULL)
		cdec_throw(Exception(EC_InvalidPtr));

	m_desc  = eDs->get_Attribute(__X("desc"))->get_Value();
	m_dburl = eConfig->get_Attribute(__X("dburl"))->get_Value();
	m_uname = eConfig->get_Attribute(__X("uname"))->get_Value();
	m_pwd = eConfig->get_Attribute(__X("pwd"))->get_Value();
	m_database = eConfig->get_Attribute(__X("database"))->get_Value();

	ASSERT(m_dburl != NULL);
	ASSERT(m_uname != NULL);
	ASSERT(m_pwd != NULL);
	ASSERT(m_database != NULL);

}
ref<Connection> DbUtil::Conn()
{
	m_conn = gc_new<Connection>(m_dburl, m_uname, m_pwd, m_database);
	return m_conn;
}
bool DbUtil::Execute(stringx sql)
{
    bool retCode = false;
    try
    {
        sql::Statement *stmt = m_conn->m_impl->createStatement();
        retCode = stmt->execute(Strx2SqlStr(sql));
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "  on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return retCode;
}
int DbUtil::ExecuteUpdate(stringx sql)
{
    int rowNum = 0;
    try
    {
        sql::Statement *stmt = m_conn->m_impl->createStatement();
        rowNum = stmt->executeUpdate(Strx2SqlStr(sql));
        stmt->close();
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "  on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return rowNum;
}

ref<ResultSet> DbUtil::ExecuteQuery(stringx sql)
{
    sql::ResultSet *res;
    try
    {
        sql::Statement *stmt = m_conn->m_impl->createStatement();
        res = stmt->executeQuery(Strx2SqlStr(sql));
        stmt->close();
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "  on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return gc_new<ResultSet>(res);
}
void DbUtil::CloseConn()
{
	m_conn->Close();
}