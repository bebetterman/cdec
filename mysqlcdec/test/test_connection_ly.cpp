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
		stringx table = __X("testXXX");

		//connect test database;
		ConnectionManager::Config config;
		config.Url = __X("tcp://192.168.16.94:3306");
		config.Username = __X("ekpapi");
		config.Password = __X("ekpapi");
		ref<ConnectionManager> cm = gc_new<ConnectionManager>(config);
		ref<Connection> conn = cm->Take();

		/*create a test table demonstrating the use of cdec::Statement.excute()*/
		ref<Statement> stmt = conn->CreateStatement();
		
		UNITTEST_ASSERT(0 == stmt->Execute(stringx(__X("USE ")) + __X("ekpapi")));
		UNITTEST_ASSERT(0 == stmt->Execute(__X("DROP TABLE IF EXISTS ") + table));
		UNITTEST_ASSERT(0 == stmt->Execute(__X("CREATE TABLE ") + table + __X("(\
			id CHAR(16), \
			name CHAR(16)\
			)")));
		Console::WriteLine(__X("#\t ") + table + __X(" table created"));
		
		/* Populate the test table with data */
		UINT64 EXAMPLE_NUM_TEST_ROWS = 5;
		const int EXAMPLE_NUM_TEST_START = 20130705;
		
		for (int i = 0; i < EXAMPLE_NUM_TEST_ROWS; ++i)
		{
			stringx sql = __X("");
			sql.Append(__X("INSERT INTO ") + table + __X("(id, name) VALUES ("));
			sql.Append(Converter::ToString(EXAMPLE_NUM_TEST_START + i));
			sql.Append(__X(", 'name"));
			sql.Append(Converter::ToString(i));
			sql.Append(__X("');"));
			UNITTEST_ASSERT(0 ==stmt->Execute(sql));
		}
		Console::WriteLine(__X("#\t Insert datas to ") + table + __X(" successfully!"));
		stringx sql = __X("select * from ") + table;
		ref<ResultSet> res = stmt->ExecuteQuery(sql);
		UNITTEST_ASSERT(res->RowsCount() == EXAMPLE_NUM_TEST_ROWS);
		int row = 0;
		while (res->Next())
		{
			UNITTEST_ASSERT(res->GetString(1) == Converter::ToString(EXAMPLE_NUM_TEST_START + row));
			UNITTEST_ASSERT(res->GetString(2) == (__X("name") + Converter::ToString(row)));
			++row;
		}
		UNITTEST_ASSERT(0 == stmt->Execute(__X("DROP TABLE IF EXISTS ") + table));
		conn->Return();
		Console::WriteLine(__X("#\t -------------- End connector/C++ connction usage unit test -----------"));
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestConnection);

// -------------------------------------------------------------------------- //
#endif
