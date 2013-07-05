#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TEST_HOST "tcp://192.168.16.44:3306"
#define TEST_USER "sxm"
#define TEST_PASS "sx^m"
#define TEST_DB   "test"

using namespace std;


#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestOperate : public UnitTestSuite
{

	UNITTEST_SUITE(TestOperate)
		UNITTEST_METHOD(testDropT) 
		UNITTEST_METHOD(testCreateT)
		UNITTEST_METHOD(testInsert)
		UNITTEST_METHOD(testUpdate)
		UNITTEST_METHOD(testSelect)
		UNITTEST_METHOD(testDelete)
		UNITTEST_METHOD(Drop)
	UNITTEST_SUITE_END()

		//Connection Parameter
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::PreparedStatement *prep_stmt;
		string sql;


public:
	void setUp()
	{
	}

    sql::Connection *testConn()
    {
        try
        {
            driver = get_driver_instance();
            con = driver->connect(TEST_HOST, TEST_USER, TEST_PASS);
            con->setSchema(TEST_DB);

        }
        catch(sql::SQLException &e)
        {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line i"            << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }

        return con;
    }
    void testDropT()
    {
			con = testConn();

			sql = "drop table if exists stu ";
			stmt = con->createStatement();
			UNITTEST_ASSERT((stmt->executeUpdate(sql)) == 0);//create add drop always return 0

			delete con;
			delete stmt;

	}
	void testCreateT()
	{
			con = testConn();

			sql = "create table stu (id int primary key,name varchar(20) not null)";
			stmt = con->createStatement();
			UNITTEST_ASSERT(stmt->executeUpdate(sql) == 0);

			delete con;
			delete stmt;

			cout << endl;
	}
	void testInsert()
	{
			con = testConn();

			sql = "insert into stu(id,name) values (99,'ning')";
			stmt = con->createStatement();
			UNITTEST_ASSERT(stmt->executeUpdate(sql) == 1);

			sql = "insert into stu(id,name) values (?,?)";
			prep_stmt = con->prepareStatement(sql);
			int n = 0;
			for(int i = 1; i <= 10; i++)
			{
            prep_stmt->setInt(1, i);
            prep_stmt->setString(2, "feng");
            n += prep_stmt->executeUpdate();
			}
			UNITTEST_ASSERT( n == 10);

			delete con;
			delete stmt;
			delete prep_stmt;
    }
    void testUpdate()
    {
			con = testConn();

			sql = "update stu set name='ningman' where id=1";
			stmt = con->createStatement();
			UNITTEST_ASSERT(stmt->executeUpdate(sql) == 1);
			
			delete con;
			delete stmt;

	}
	void testSelect()
	{
			con = testConn();

			sql = "select name from stu where id = 1";
			res = (con->createStatement())->executeQuery(sql);
			string x;
			while(res->next())
			{
            x = res->getString("name");
			}
			std::cout << "id = 1,name = " << x << endl;
			UNITTEST_ASSERT(x == "ningman");

			sql = "select * from stu ";
			res = (con->createStatement())->executeQuery(sql);
			while(res->next())
			{
            cout << "id = " << res->getInt("id");
            cout << " ;name = " << res->getString("name") << endl;
			}

			delete con;
			delete stmt;
	}

	void testDelete()
	{
			con = testConn();

			sql = "delete from stu where id = 99";
			stmt = con->createStatement();
			UNITTEST_ASSERT(stmt->executeUpdate(sql) == 1);

			delete con;
			delete stmt;
	}

	
	void Drop()
	{
			con = testConn();

			stmt = con->createStatement();
			UNITTEST_ASSERT(stmt->executeUpdate("drop table stu") == 0);

			delete con;
			delete stmt;

	}
	void tearDown()
	{
	}

};


UNITTEST_SUITE_REGISTRATION(TestOperate);

// -------------------------------------------------------------------------- //
#endif