#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class Test_Ning : public UnitTestSuite
{
	UNITTEST_SUITE(Test_Ning)
		UNITTEST_METHOD(test_ly)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	

	void test_ly(){

			//Connection Parameter
			stringx url = __X("tcp://192.168.16.44:3306");
			stringx use = __X("sxm");
			stringx psw = __X("sx^m");
			stringx db = __X("test");
			
	
			//connect to DB
			ref<Connection> conn=gc_new<Connection>(url,use,psw);
			
			ref<Statement> stmt=conn->CreateStatement();

			UNITTEST_ASSERT(0 == stmt->Execute(__X("USE ") + db));
			UNITTEST_ASSERT(0 == stmt->Execute(__X("drop table if exists ttt")));

			std::cout<<"1111"<<std::endl;
			UNITTEST_ASSERT(0 == stmt->Execute(__X("create table ttt(id int(4) primary key,name varchar(20))")));
			std::cout<<"2222"<<std::endl;
			stringx sql=__X("insert into ttt(id,name) values(999,'ning')");
			UNITTEST_ASSERT(0 == stmt->Execute(sql));
			std::cout<<"3333"<<std::endl;
				
			sql = __X("update ttt set name = 'ningman' where id = 999");
			UNITTEST_ASSERT(0 == stmt->Execute(sql));
			std::cout<<"4444"<<std::endl;

			sql = __X("select name from ttt where id = 999");
			ref<ResultSet> res=stmt->ExecuteQuery(sql);
			stringx c = __X("ningman");
			
			while(res->Next())
			{
				 UNITTEST_ASSERT(c == res->GetString(1));
				  std::cout<<"5555"<<std::endl;
				
			}
		   
			UNITTEST_ASSERT(stmt->Execute(__X("drop table ttt")) == 0);
			std::cout<<"6666"<<std::endl;

			Console::WriteLine(__X("#\t Insert datas to ttt successfully!"));
		



	
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(Test_Ning);

// -------------------------------------------------------------------------- //
#endif

