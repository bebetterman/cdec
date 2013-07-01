#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS
	stringx testdir = __X("..\\tests\\testdir");
#else
	stringx testdir = __X("../tests/testdir");
#endif

// -------------------------------------------------------------------------- //

class TestCdecDirectory : public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecDirectory)
		UNITTEST_METHOD(testGetDirectories)
		UNITTEST_METHOD(testGetFiles)
		UNITTEST_METHOD(testGetEntries)
		UNITTEST_METHOD(testDirectoryIO)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testGetDirectories()
	{
		// TODO 此案例在 Linux 下无法通过
		ref<StringArrayList> dirs = Directory::GetDirectories(testdir);

//       std::wcout << dirs->Count() << std::endl;
//		std::wcout << std::endl;
//	    for (size_t i = 0, n = dirs->Count(); i < n; ++i)
//		{
//			std::wcout << __X("[") << dirs->at(i).ToStdString() << __X("]\t"));
//		}
//		std::wcout << std::endl;

#ifdef X_OS_WINDOWS
		UNITTEST_ASSERT(dirs->Count() == 5);
#else
                UNITTEST_ASSERT(dirs->Count() == 6);	// .svn
#endif
	}

	void testGetFiles()
	{
		ref<StringArrayList> files = Directory::GetFiles(testdir);

//        std::wcout << files->Count() << std::endl;
//		std::wcout << std::endl;
//		for (size_t i = 0, n = files->Count(); i < n; ++i)
//		{
//			std::wcout << __X("[") << files->at(i).ToStdString() << __X("]\t"));
//		}
//		std::wcout << std::endl;

		UNITTEST_ASSERT(files->Count() == 3);
	}

	void testGetEntries()
	{
		ref<StringArrayList> entries = Directory::GetFileSystemEntries(testdir);

//        std::wcout << entries->Count() << std::endl;
//		std::wcout << std::endl;
//		for (size_t i = 0, n = entries->Count(); i < n; ++i)
//		{
//			std::wcout << __X("[") << entries->at(i).ToStdString() << __X("]\t"));
//		}
//		std::wcout << std::endl;

#ifdef X_OS_WINDOWS
		UNITTEST_ASSERT(entries->Count() == 8);
#else
                UNITTEST_ASSERT(entries->Count() == 9);	// Including .svn
#endif
	}

	void testDirectoryIO()
	{
		stringx newdir1 = __X("newdir1");
		stringx newdir2 = __X("newdir2");

		Directory::Create(newdir1);
		Directory::Create(newdir2);

		Directory::Delete(newdir1);

		Directory::Move(newdir2, newdir1);

		UNITTEST_ASSERT(Directory::Exists(newdir1));

		stringx currentdir = Directory::GetCurrentDir();
//		std::wcout << currentdir.c_str() << std::endl;
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecDirectory);

// -------------------------------------------------------------------------- //
#endif
