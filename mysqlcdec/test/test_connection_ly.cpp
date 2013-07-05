#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestConnection : public UnitTestSuite
{
	UNITTEST_SUITE(TestConnection)
		UNITTEST_METHOD(testMysqlConn)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{

	}

	void testMysqlConn()
	{
		Console::WriteLine(__X("#\t -------------- Start connector/C++ connction usage unit test -----------"));
		//UNITTEST_ASSERT(1 == 0);
		//stringx confPath = TestEnv::get_sample_path(__X("dbconfig_ly.xml"));
		stringx url = __X("tcp://192.168.137.88:3306");
		stringx name = __X("root");
		stringx pwd = __X("openstack");
		stringx db = __X("test");

		//connect test database;
		ref<Connection> conn = gc_new<Connection>(url, name, pwd);

		/*create a test table demonstrating the use of cdec::Statement.excute()*/
		ref<Statement> stmt = conn->CreateStatement();
		
		UNITTEST_ASSERT(0 == stmt->execute(__X("USE ") + db));
		UNITTEST_ASSERT(0 == stmt->execute(__X("DROP TABLE IF EXISTS testXXX")));
		UNITTEST_ASSERT(0 == stmt->execute(__X("CREATE TABLE testXXX(\
			id CHAR(16), \
			name CHAR(16)\
			)")));
		Console::WriteLine(__X("#\t testXXX table created"));
		
		/* Populate the test table with data */
		UINT64 EXAMPLE_NUM_TEST_ROWS = 5;
		const int EXAMPLE_NUM_TEST_START = 20130705;
		
		for (int i = 0; i < EXAMPLE_NUM_TEST_ROWS; ++i)
		{
			stringx sql = __X("");
			sql.Append(__X("INSERT INTO testXXX(id, name) VALUES ("));
			sql.Append(Converter::ToString(EXAMPLE_NUM_TEST_START + i));
			sql.Append(__X(", 'name"));
			sql.Append(Converter::ToString(i));
			sql.Append(__X("');"));
			UNITTEST_ASSERT(0 ==stmt->execute(sql));
		}
		Console::WriteLine(__X("#\t Insert datas to testXXX table successfully!"));
		stringx sql = __X("select * from testXXX");
		ref<ResultSet> res = stmt->executeQuery(sql);
		UNITTEST_ASSERT(res->RowsCount() == EXAMPLE_NUM_TEST_ROWS);
		int row = 0;
		while (res->Next())
		{
			UNITTEST_ASSERT(res->getString(1) == Converter::ToString(EXAMPLE_NUM_TEST_START + row));
			UNITTEST_ASSERT(res->getString(2) == (__X("name") + Converter::ToString(row)));
			++row;
		}
		conn->Close();
		Console::WriteLine(__X("#\t -------------- End connector/C++ connction usage unit test -----------"));
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestConnection);

// -------------------------------------------------------------------------- //
#endif
