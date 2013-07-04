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
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testSimpleConn()
	{
		cout << endl;

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
				cout << "id = ";
				cout << res->getString(1) << endl;
				cout << "name = ";
				cout << res->getString("name") << endl;
			}
			delete res;
			delete stmt;
			delete con;

		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line i"            << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}

		cout << endl;
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestRawBasic);

// -------------------------------------------------------------------------- //
#endif
