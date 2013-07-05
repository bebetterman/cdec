#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //
struct StringxSpy: stringx
{
	using stringx::m_pContent;
};
class TestRawBasic : public UnitTestSuite
{
	UNITTEST_SUITE(TestRawBasic)
		UNITTEST_METHOD(testSimpleConn)
		UNITTEST_METHOD(testPrepareStatement)
		

	UNITTEST_SUITE_END()

public:
	inline StringContent* getStringContent(stringx& s)
	{
		return ((StringxSpy&)s).m_pContent;
	}
	void setUp()
	{
	}
	void testPrepareStatement()
	{
		stringx confPath=TestEnv::get_sample_path(__X("dbconfig.xml"));
		ref<DbUtil> dbutil = gc_new<DbUtil>(confPath);
		dbutil->Conn();
		dbutil->Execute(__X("drop table if exists stu"));
		dbutil->Execute(__X("create table stu(idd int(4),name varchar(20),primary key(idd))"));
		dbutil->Execute(__X("delete from stu"));
		dbutil->Execute(__X("insert into stu values(1,'test')"));
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
	}
	void testSimpleConn()
	{
		std::cout << std::endl;

		try {
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
			res = stmt->executeQuery("SELECT * from stu");
			while (res->next()) {
				std::cout << "id = ";
				std::cout << res->getString(1) << std::endl;
				std::cout << "name = ";
				std::cout << res->getString("name") << std::endl;
			}
			delete res;
			delete stmt;
			delete con;

		} catch (sql::SQLException &e) {
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
