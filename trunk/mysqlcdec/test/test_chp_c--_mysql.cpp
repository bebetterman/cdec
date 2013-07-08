#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST

class TestMysql_C : public UnitTestSuite
{
	UNITTEST_SUITE(TestMysql_C)
		UNITTEST_METHOD(testConnect)
		UNITTEST_METHOD(testDrop)
		UNITTEST_METHOD(testCreate)
		UNITTEST_METHOD(testInsert)
		UNITTEST_METHOD(testDelete)
		UNITTEST_METHOD(testUpdate)
		UNITTEST_METHOD(testSelect)
		UNITTEST_METHOD(testPrepare)
	UNITTEST_SUITE_END()
public:
	void setUp()
	{
	}
	void testConnect()
	{
		sql::Driver *driver = get_driver_instance();
		sql::Connection *conn = driver->connect("tcp://192.168.137.88:3306","root","openstack");
	}
	void testDrop()
	{
		stringx configPath = TestEnv::get_sample_path(__X("dbconfig_ly.xml"));
		ref<DbUtil> dbUtil = gc_new<DbUtil>(configPath);
		ref<Connection> conn = dbUtil->Conn();
		
		UNITTEST_ASSERT(!dbUtil->Execute(__X("drop table  IF EXISTS class")));
		dbUtil->CloseConn();
	}
	void testCreate()
	{
		stringx configPath = TestEnv::get_sample_path(__X("dbconfig_ly.xml"));
		ref<DbUtil> dbUtil = gc_new<DbUtil>(configPath);
		dbUtil->Conn();
		UNITTEST_ASSERT(!dbUtil->ExecuteUpdate(__X("create table class(id int(4) not null primary key,name char(20) not null)")));
		dbUtil->CloseConn();
			 
	}
	void testInsert()
	{
		stringx configPath = TestEnv::get_sample_path(__X("dbconfig_ly.xml"));
		ref<DbUtil> dbUtil = gc_new<DbUtil>(configPath);
		ref<PrepareStatement> prestmt = dbUtil->Conn()->CreatePrepareStatement(__X("insert into class values(?,?)"));
		for(int i=0;i<4;i++){
			prestmt->SetInt(1,i);
			prestmt->SetString(2,__X("class"));
			UNITTEST_ASSERT(prestmt->ExecuteUpdate());
		}
		dbUtil->CloseConn();
	}
	void testDelete()
	{
		stringx configPath = TestEnv::get_sample_path(__X("dbconfig_ly.xml"));
		ref<DbUtil> dbUtil = gc_new<DbUtil>(configPath);
		dbUtil->Conn();
		UNITTEST_ASSERT(dbUtil->ExecuteUpdate(__X("delete from class where id=2")));
		dbUtil->CloseConn();
	}
	void testUpdate()
	{
		stringx configPath = TestEnv::get_sample_path(__X("dbconfig_ly.xml"));
		ref<DbUtil> dbUtil = gc_new<DbUtil>(configPath);
		dbUtil->Conn();
		UNITTEST_ASSERT(dbUtil->ExecuteUpdate(__X("update class set name = 'one two' where id=1")));
		dbUtil->CloseConn();
	}
	void testSelect()
	{
		stringx configPath = TestEnv::get_sample_path(__X("dbconfig_ly.xml"));
		ref<DbUtil> dbUtil = gc_new<DbUtil>(configPath);
		dbUtil->Conn();
		ref<ResultSet> rset = dbUtil->ExecuteQuery(__X("select * from class where id = 3"));
		UNITTEST_ASSERT(rset->RowsCount() == 1);
		while(rset->Next())
		{
			stringx s = __X("class");
			stringx w = rset->GetString(2);
			UNITTEST_ASSERT(s == w);
		}
		dbUtil->CloseConn();
	}
	void testPrepare()
	{
		stringx configPath = TestEnv::get_sample_path(__X("dbconfig_ly.xml"));
		ref<DbUtil> dbUtil = gc_new<DbUtil>(configPath);
		dbUtil->Conn();
		ref<PrepareStatement> prestmt = dbUtil->Conn()->CreatePrepareStatement(__X("insert into class values(?,?)"));
		for(int i=4;i<10;i++){
			prestmt->SetInt(1,i);
			 prestmt->SetString(2,__X("prepare"));
			 prestmt->ExecuteUpdate();
		}
		ref<ResultSet> rset = dbUtil->ExecuteQuery(__X("select * from class"));
		UNITTEST_ASSERT(rset->RowsCount() == 9);
		dbUtil->CloseConn();
	}
	void tearDown()
	{
	}

};

UNITTEST_SUITE_REGISTRATION(TestMysql_C);
#endif