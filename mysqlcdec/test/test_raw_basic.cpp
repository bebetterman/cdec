#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestRawBasic : public UnitTestSuite
{
	UNITTEST_SUITE(TestRawBasic)
		UNITTEST_METHOD(testSimpleConn)
		UNITTEST_METHOD(testDbUtil)
		

	UNITTEST_SUITE_END()
public:
	void setUp()
	{
	}
	void testDbUtil()
	{
		stringx confPath=TestEnv::get_sample_path(__X("dbconfig.xml"));
		ref<DbUtil> dbutil = gc_new<DbUtil>(confPath);
		dbutil->Conn();
		dbutil->Execute(__X("drop table if exists stu"));
		dbutil->Execute(__X("create table stu(idd int(4),name varchar(20),primary key(idd))"));
		dbutil->Execute(__X("delete from stu"));
		dbutil->Execute(__X("insert into stu values(1,'test')"));
		ref<ResultSet> resu = dbutil->ExecuteQuery(__X("select * from stu"));
		UNITTEST_ASSERT(resu->RowsCount() == 1);
		while(resu->Next())
		{
			stringx s = __X("test");
			stringx w =  resu->getString(2);
			UNITTEST_ASSERT(s == w);
		}
		ref<PrepareStatement> prstmt  = dbutil->GetConn()->prepareStatement(__X("select * from stu where idd=?"));
		prstmt->SetInt(1,1);
		ref<ResultSet> res = prstmt->ExecuteQuery();
		UNITTEST_ASSERT(res->RowsCount() == 1);
		while(res->Next())
		{
			stringx s = __X("test");
			stringx w =  res->getString(2);
			UNITTEST_ASSERT(s == w);
		}
		//dbutil->CloseConn();
		prstmt  = dbutil->GetConn()->prepareStatement(__X("insert into stu values(? ,?)"));
		for( int i=2; i<10; i++)
		{
			prstmt->SetInt(1,i);
			prstmt->SetString(2,Converter::ToString(i));
			prstmt->ExecuteUpdate();
		}

		dbutil->CloseConn();
	}
	void testSimpleConn()
	{
		std::cout << std::endl;

        try
        {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stmt;
            sql::ResultSet *res;

			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect("tcp://192.168.16.44:3306", "sxm", "sx^m");
			/* Connect to the MySQL test database */
			con->setSchema("sxm");

            stmt = con->createStatement();
            std::string sql = "Welcome to Connector/C++";
            res = stmt->executeQuery("SELECT 'Welcome to Connector/C++' AS _message");
            while (res->next())
            {
                std::string mess = res->getString("_message");
                UNITTEST_ASSERT(sql == mess);
            }
            delete res;
            delete stmt;
            delete con;

		}
		catch (sql::SQLException &e)
		{
			std::cout << "# ERR: SQLException in " << __FILE__;
			std::cout << "(" << __FUNCTION__ << ") on line i"            << __LINE__ << std::endl;
			std::cout << "# ERR: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}

		std::cout << std::endl;
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestRawBasic);

// -------------------------------------------------------------------------- //
#endif
