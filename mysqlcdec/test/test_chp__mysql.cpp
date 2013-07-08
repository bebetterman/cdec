#include "stdafx.h"
#include "testenv.h"
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST

class TestMysql : public UnitTestSuite
{
	UNITTEST_SUITE(TestMysql)
		UNITTEST_METHOD(testConnect)
		UNITTEST_METHOD(testDropTable)
		UNITTEST_METHOD(testCreateTable)
		UNITTEST_METHOD(testInsert)
		UNITTEST_METHOD(testSelect)
		UNITTEST_METHOD(testUpdate)
		UNITTEST_METHOD(testDelete)
		UNITTEST_METHOD(testPrepareStatement)
		UNITTEST_METHOD(testDropTable)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}
	sql::Connection * getConnection()
	{
		sql::Driver * driver;
		sql::Connection * conn;

		driver =  get_driver_instance();
		conn = driver->connect("tcp://192.168.16.44:3306","sxm","sx^m");

		conn->setSchema("sxm");
		
		return conn;
	}
	void testConnect()
	{
		sql::Connection *conn = getConnection();
		
	}
	void testDropTable()
	{
		sql::Connection * conn = getConnection();
		sql::Statement * stmt = conn->createStatement();

		UNITTEST_ASSERT(!stmt->execute("drop table IF EXISTS staff"));

		stmt->close();
		conn->close();
	}
	void testCreateTable()
	{
		sql::Connection *conn = getConnection();
		sql::Statement *stmt = conn->createStatement();
		sql::ResultSet *rset;
		string sql = "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_SCHEMA='sxm' and TABLE_NAME='staff'";
		rset = stmt->executeQuery(sql);

        if(!rset->next())
        {
            UNITTEST_ASSERT(!stmt->execute("create table staff(id int(4) not null,name varchar(20) not null,primary key(id))"));
            stmt->close();
            conn->close();
        }

    }
    void testInsert()
    {
        sql::Connection *conn;
        sql::PreparedStatement *prestmt;

        conn = getConnection();
        prestmt = conn->prepareStatement("insert into staff values(?,?)");
        for(int i = 1; i < 10; i++)
        {
            prestmt->setInt(1, i);
            prestmt->setString(2, "chp");

			UNITTEST_ASSERT(!prestmt->execute());
		}
		prestmt->close();
		conn->close();
	}
	void testSelect()
	{
		sql::Connection * conn = getConnection();
     	sql::Statement * statement = conn->createStatement();
		sql::ResultSet * rset = statement->executeQuery("select * from staff");

        UNITTEST_ASSERT(rset->rowsCount() == 9);
        while(rset->next())
        {
            string s = "chp";
            string w = rset->getString(2);
			cout << rset->rowsCount() << rset->getInt(1) << "   "<< rset->getString(2) <<endl;

			 UNITTEST_ASSERT(s == w);
		 }

        rset->close();
        statement->close();
        conn->close();

    }
    void testUpdate()
    {
        try
        {
            sql::Connection *conn = getConnection();
            sql::Statement *statement = conn->createStatement();
            sql::ResultSet *rset;
            UNITTEST_ASSERT(!statement->execute("update staff set name = 'test' where id = 3"));

            rset =  statement->executeQuery("select * from staff where id = 3 ");
			while(rset->next())
			{
				 string w=rset->getString(2);
				 string s = "test";
				 UNITTEST_ASSERT( s == w);
			}
			rset->close();
            statement->close();
            conn->close();
        }
        catch(sql::SQLException &e)
        {
            cout << "update error" << e.getErrorCode() << endl;
        }
    }
    void testDelete()
    {
        try
        {
            sql::Connection *conn = getConnection();
            sql::Statement *statement = conn->createStatement();

			 bool a=statement->execute("delete from staff where id = 5");

		    UNITTEST_ASSERT(!statement->execute("delete from staff where id = 5"));

            statement->close();
            conn->close();
        }
        catch(sql::SQLException &e)
        {
            cout << "delete error!" << endl;
        }
    }
    void testPrepareStatement()
    {
        sql::Connection *conn = getConnection();
        sql::PreparedStatement *prestmt;
		sql::ResultSet *rset;

        prestmt = conn->prepareStatement("select * from staff where id = ?");
        prestmt->setInt(1 ,2);
        rset = prestmt->executeQuery();
        while(rset->next())
        {
            string s = "chp";
            string w = rset->getString(2);
            UNITTEST_ASSERT(s == w);
        }
        rset->close();
        prestmt->close();
        conn->close();
    }
    void testDrop()
    {
        sql::Connection *conn = getConnection();
        sql::Statement *stmt = conn->createStatement();

		UNITTEST_ASSERT(!stmt->execute("drop table IF EXISTS staff"));

		stmt->close();
		conn->close();
	}
	void tearDown()
	{
	}
};
UNITTEST_SUITE_REGISTRATION(TestMysql);

#endif