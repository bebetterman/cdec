#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST

class TestMysqlCdec : public UnitTestSuite
{
	UNITTEST_SUITE(TestMysqlCdec)
		UNITTEST_METHOD(testDrop)
		UNITTEST_METHOD(testCreate)
		UNITTEST_METHOD(testInsert)
		UNITTEST_METHOD(testDelete)
		UNITTEST_METHOD(testUpdate)
		UNITTEST_METHOD(testSelect)
		UNITTEST_METHOD(testPrepare)
	UNITTEST_SUITE_END()

public:
	ref<ConnectionManager> m_cm;
	ref<Connection> m_conn;

	void setUp()
	{
		stringx pathConfig = TestEnv::get_sample_path(__X("dbconfig_ly.xml"));
		m_cm = gc_new<ConnectionManager>(pathConfig);
		m_conn = m_cm->Take();
	}

	void testDrop()
	{
		UNITTEST_ASSERT(!m_conn->Execute(__X("drop table  IF EXISTS class")));
	}

	void testCreate()
	{
		UNITTEST_ASSERT(!m_conn->ExecuteUpdate(__X("create table class(id int(4) not null primary key,name char(20) not null)")));
			 
	}

	void testInsert()
	{
		ref<PreparedStatement> prestmt = m_conn->CreatePreparedStatement(__X("insert into class values(?,?)"));
		for(int i=0;i<4;i++){
			prestmt->SetInt(1,i);
			prestmt->SetString(2,__X("class"));
			UNITTEST_ASSERT(prestmt->ExecuteUpdate());
		}
	}

	void testDelete()
	{
		UNITTEST_ASSERT(m_conn->ExecuteUpdate(__X("delete from class where id=2")));
	}

	void testUpdate()
	{
		UNITTEST_ASSERT(m_conn->ExecuteUpdate(__X("update class set name = 'one two' where id=1")));
	}

	void testSelect()
	{
		ref<ResultSet> rset = m_conn->ExecuteQuery(__X("select * from class where id = 3"));
		UNITTEST_ASSERT(rset->RowsCount() == 1);
		while(rset->Next())
		{
			stringx s = __X("class");
			stringx w = rset->GetString(2);
			UNITTEST_ASSERT(s == w);
		}
	}

	void testPrepare()
	{
		ref<PreparedStatement> prestmt = m_conn->CreatePreparedStatement(__X("insert into class values(?,?)"));
		for(int i=4;i<10;i++){
			prestmt->SetInt(1,i);
			 prestmt->SetString(2,__X("prepare"));
			 prestmt->ExecuteUpdate();
		}
		ref<ResultSet> rset = m_conn->ExecuteQuery(__X("select * from class"));
		UNITTEST_ASSERT(rset->RowsCount() == 9);
	}

	void tearDown()
	{
		m_cm->Return(m_conn);
		m_conn = NULL;

		m_cm->Close();
		m_cm = NULL;
	}
};

UNITTEST_SUITE_REGISTRATION(TestMysqlCdec);

#endif