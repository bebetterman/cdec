#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestFileMapping: public UnitTestSuite
{
	UNITTEST_SUITE(TestFileMapping)
		UNITTEST_METHOD(testMapBasic)
		UNITTEST_METHOD(testMapWin32Basic)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testMapBasic()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("test_mapping.bin"));
		File::Delete(sampleFile);

		ref<FileStream> fs = gc_new<FileStream>(sampleFile, FileStream::AccessReadWrite, true);
		const int n = 10000;
		fs->SetLength(n * 4096);
		for (int i = 0; i < n; ++i)
		{
			fs->Seek(i * 4096);
			fs->Write(&i, 4);
		}

		ref<FileMapping> map = gc_new<FileMapping>();
		BYTE* pbase = (BYTE*)map->Open(fs, true, 0);
		BYTE* p = pbase;
		for (int i = 0; i < n; ++i)
		{
			UNITTEST_ASSERT(*(int*)p == i);
			*(int*)p = -i;
			p += 4096;
		}

		map->Close();

		int value;
		for (int i = 0; i < n; ++i)
		{
			fs->Seek(i * 4096);
			UNITTEST_ASSERT(fs->Read(&value, 4) == 4);
			UNITTEST_ASSERT(value == -i);
		}

		fs->Close();
		File::Delete(sampleFile);
	}

	void testMapWin32Basic()
	{
#ifdef X_OS_WINDOWS
		stringx sampleFile = TestEnv::get_sample_path(__X("test_mapping.bin"));
		File::Delete(sampleFile);

		ref<FileStream> fs = gc_new<FileStream>(sampleFile, FileStream::AccessReadWrite, true);
		const int n = 10000;
		fs->SetLength(n * 4096);
		for (int i = 0; i < n; ++i)
		{
			fs->Seek(i * 4096);
			fs->Write(&i, 4);
		}

		ref<FileMappingWin32> map = gc_new<FileMappingWin32>(fs, true);
		BYTE* pbase = (BYTE*)map->MapView(0, 0, true);
		BYTE* p = pbase;
		for (int i = 0; i < n; ++i)
		{
			UNITTEST_ASSERT(*(int*)p == i);
			*(int*)p = -i;
			p += 4096;
		}

		map->UnmapView(pbase);
		map->Close();

		int value;
		for (int i = 0; i < n; ++i)
		{
			fs->Seek(i * 4096);
			UNITTEST_ASSERT(fs->Read(&value, 4) == 4);
			UNITTEST_ASSERT(value == -i);
		}

		fs->Close();
		File::Delete(sampleFile);
#endif
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestFileMapping);

// -------------------------------------------------------------------------- //
#endif
