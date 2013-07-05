#include "stdafx.h"
#include "prepareStatement.h"

// -------------------------------------------------------------------------- //
// prepareStatement
// -------------------------------------------------------------------------- //
void PrepareStatement::Close()
{
    try
    {
        if(m_impl != NULL)
            m_impl->close();
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "  on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
}
int PrepareStatement::ExecuteUpdate()
{
    int rowNum = 0;
    try
    {
        rowNum = m_impl->executeUpdate();
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
ref<ResultSet> PrepareStatement::ExecuteQuery()
{
    sql::ResultSet   *res = NULL;
    try
    {
        res = m_impl->executeQuery();
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

void PrepareStatement::SetString(UINT index, stringx value)
{
	ref<Encoding>	 encode = Encoding::get_UTF8();
	std::string stdSql = encode->FromUnicode(value);
	m_impl->setString(index,stdSql);
}

void PrepareStatement::SetInt(UINT index ,INT value)
{
	m_impl->setInt(index,value);
}