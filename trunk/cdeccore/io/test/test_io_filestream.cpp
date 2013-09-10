#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestTextStream: public UnitTestSuite
{
	UNITTEST_SUITE(TestTextStream)
		UNITTEST_METHOD(testFileExist)
		UNITTEST_METHOD(testOpenStream)
	//	UNITTEST_METHOD_EXCEPTION(testOpenFailed)		// TODO pure virtual function call
		UNITTEST_METHOD(testReadStream)
		UNITTEST_METHOD(testReadStreamToBytes)
		UNITTEST_METHOD(testAtomReadStream)
		UNITTEST_METHOD(testSeekStream)
		UNITTEST_METHOD(testWriteStream)
		UNITTEST_METHOD(testWriteStreamFromBytes)
		UNITTEST_METHOD(testAtomWriteStream)
		UNITTEST_METHOD(testSetEnd)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testFileExist()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		UNITTEST_ASSERT(File::Exists(sampleFile));

		sampleFile = TestEnv::get_sample_path(__X("fake_file.txt"));
		UNITTEST_ASSERT(!File::Exists(sampleFile));
	}

	void testOpenStream()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessRead, false);

		const char c[4] = { 'H', 'e', 'l', 'l' };
		DWORD buf;
		UNITTEST_ASSERT(pStrm->Read(&buf, sizeof(buf)) == sizeof(buf));
		UNITTEST_ASSERT(buf == *(DWORD*)c);

		INT64 length = pStrm->Length();
		UNITTEST_ASSERT(length == 30);
		INT64 pos = pStrm->Position();
		UNITTEST_ASSERT(pos == 4);
	}

	void testOpenFailed()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("fake_file.txt"));
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessRead, false);
	}

	void testReadStream()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessRead, false);
		INT64 length = pStrm->Length();
		UNITTEST_ASSERT(pStrm->Position() == 0);

		const UINT cbBuf = 8;
		BYTE	buf[cbBuf];
		while (length > 0)
		{
			UINT cbRead = pStrm->Read(buf, cbBuf);
			UNITTEST_ASSERT((length < cbBuf ? length : cbBuf) == cbRead);
			length -= cbRead;
		}
		UNITTEST_ASSERT(length == 0);
		UNITTEST_ASSERT(pStrm->Position() == pStrm->Length());
	}

	void testReadStreamToBytes()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessRead, false);
		INT64 length = pStrm->Length();
		UNITTEST_ASSERT(pStrm->Position() == 0);

		ref<ByteArray> buffer = gc_new<ByteArray>(8);
		int bufferSize = buffer->Count();
		while (length > 0)
		{
			UINT cbRead = pStrm->Read2(buffer, 0, bufferSize);
			UNITTEST_ASSERT((length < bufferSize ? length : bufferSize) == cbRead);
			length -= cbRead;
		}
		UNITTEST_ASSERT(length == 0);
		UNITTEST_ASSERT(pStrm->Position() == pStrm->Length());
	}

	void testAtomReadStream()
	{
#ifndef X_OS_WINDOWS
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessRead, false);
		INT64 length = pStrm->Length();
		UNITTEST_ASSERT(pStrm->Position() == 0);
		//printf("length=%d",length);
		int cbBuf = 1;
		BYTE	buf[1];
		char c[] = {'H','e','l','l','o'};
		for( int i=4 ; i>=0;i--)
		{
			int len = pStrm->AtomRead(i, &buf, cbBuf);
			UNITTEST_ASSERT(len==cbBuf);
			//printf("buf=%c\n",*(BYTE *)buf);
			//printf("c=%c\n",c[i]);
			UNITTEST_ASSERT(*(BYTE *)buf==c[i]);
		}
#endif
	}

	void testSeekStream()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessRead, false);
		INT64 length = pStrm->Length();

		// 正常的 Seek
		UNITTEST_ASSERT(pStrm->Seek(8, Stream::SeekBegin) == 8);
		UNITTEST_ASSERT(pStrm->Seek(4, Stream::SeekCurrent) == 12);
		UNITTEST_ASSERT(pStrm->Seek(-8, Stream::SeekCurrent) == 4);
		UNITTEST_ASSERT(pStrm->Seek(-8, Stream::SeekEnd) == length - 8);

		// 开始边界
		UNITTEST_ASSERT(pStrm->Seek(-length, Stream::SeekEnd) == 0);
		UNITTEST_ASSERT_EXCEPTION(pStrm->Seek(-2, Stream::SeekCurrent));

		// 结束边界
		UNITTEST_ASSERT(pStrm->Seek(length, Stream::SeekBegin) == length);
		UNITTEST_ASSERT(pStrm->Seek(8, Stream::SeekCurrent) == length + 8);
		UNITTEST_ASSERT(pStrm->Seek(4, Stream::SeekCurrent) == length + 12);
		UNITTEST_ASSERT(pStrm->Seek(-16, Stream::SeekCurrent) == length - 4);
	}

	void testWriteStream()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("txtw_samp.txt"));
		File::Delete(sampleFile);
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessWrite, true);
		UNITTEST_ASSERT(pStrm->Length() == 0);

		const char text[] = "This is a sample.\r\n";
		const int len = sizeof(text) / sizeof(text[0]) - 1;
		UNITTEST_ASSERT(pStrm->Write(text, len) == len);
		UNITTEST_ASSERT(pStrm->Length() == len);

		pStrm->Seek(5, Stream::SeekBegin);
		UNITTEST_ASSERT(pStrm->Write(text, len) == len);
		UNITTEST_ASSERT(pStrm->Length() == len + 5);

		pStrm->Close();

		// 重新打开文件检查
		char text2[len];
		pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessReadWrite, false);
		UNITTEST_ASSERT(pStrm->Length() == len + 5);
		pStrm->Seek(5, Stream::SeekBegin);
		UNITTEST_ASSERT(pStrm->Read(text2, len) == len);
		UNITTEST_ASSERT(memcmp(text, text2, len) == 0);

		pStrm->Close();

		UNITTEST_ASSERT(File::Exists(sampleFile));
		UNITTEST_ASSERT(File::Delete(sampleFile));
	}

	void testWriteStreamFromBytes()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("txtw_samp.txt"));
		File::Delete(sampleFile);
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessWrite, true);
		UNITTEST_ASSERT(pStrm->Length() == 0);

		const char text[] = "This is a sample.\r\n";
		const int len = sizeof(text) - 1;
		ref<ByteArray> text_binary = gc_new<ByteArray>((const BYTE*)text, len);

		UNITTEST_ASSERT(pStrm->Write2(text_binary, 0, len) == len);
		UNITTEST_ASSERT(pStrm->Length() == len);

		pStrm->Seek(5, Stream::SeekBegin);
		UNITTEST_ASSERT(pStrm->Write2(text_binary, 0, len) == len);
		UNITTEST_ASSERT(pStrm->Length() == len + 5);

		pStrm->Close();

		// 重新打开文件检查
		char text2[len];
		pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessReadWrite, false);
		UNITTEST_ASSERT(pStrm->Length() == len + 5);
		pStrm->Seek(5, Stream::SeekBegin);
		UNITTEST_ASSERT(pStrm->Read(text2, len) == len);
		UNITTEST_ASSERT(memcmp(text, text2, len) == 0);

		pStrm->Close();

		UNITTEST_ASSERT(File::Exists(sampleFile));
		UNITTEST_ASSERT(File::Delete(sampleFile));
	}

	void testAtomWriteStream()
	{
#ifndef X_OS_WINDOWS
		stringx sampleFile = TestEnv::get_sample_path(__X("txtw_samp_AtomWrite.txt"));
		File::Delete(sampleFile);
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessWrite, true);
		UNITTEST_ASSERT(pStrm->Length() == 0);
		const char text[] = "This is a sample.\0";
		const int len = sizeof(text) / sizeof(text[0]) - 1;
		pStrm->SetLength(len*4096);
		for( int i = 0 ; i < len ; i++ )
		{
			UNITTEST_ASSERT(pStrm->AtomWrite(i*4096, text, len) == len);
		}
		UNITTEST_ASSERT(pStrm->Length() == len*4096);
		pStrm->Close();
		// 重新打开文件检查
		char text2[len];
		pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessRead, false);
		UNITTEST_ASSERT(pStrm->Length() == len*4096);
		for( int i = 0 ; i < len ; i++ )
		{
			//pStrm->Seek(i*4096);
			//int readlen = pStrm->Read(text2, len);
			int readlen = pStrm->AtomRead(i*4096, text2, len);
			UNITTEST_ASSERT(readlen == len);
			//printf("\n@@@%s@@@\n",text2);
			UNITTEST_ASSERT(memcmp(text, text2, len) == 0);
			
		}

		pStrm->Close();

		UNITTEST_ASSERT(File::Exists(sampleFile));
		UNITTEST_ASSERT(File::Delete(sampleFile));
#endif
	}

	void testSetEnd()
	{
		stringx sampleFile = TestEnv::get_sample_path(__X("txtw_samp.txt"));
		File::Delete(sampleFile);
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessWrite, true);
		// pStrm->Seek(4096, Stream::SeekBegin);
		pStrm->SetLength(4096);
		UNITTEST_ASSERT(pStrm->Length() == 4096);

#ifdef X_OS_LINUX
		pStrm->Seek(0, Stream::SeekEnd);
#endif

		DWORD o = 0x34333231;
		pStrm->Write(&o, sizeof(o));			// @4096
		pStrm->Seek(0, Stream::SeekBegin);
		pStrm->Write(&o, sizeof(o));			// @0
		UNITTEST_ASSERT(pStrm->Length() == 4100);

		UNITTEST_ASSERT(pStrm->Position() == 4);
		pStrm->SetLength(4);
		UNITTEST_ASSERT(pStrm->Length() == 4);

		pStrm->Close();

		// 重新打开一次
		pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessReadWrite, true);
		UNITTEST_ASSERT(pStrm->Length() == 4);
		UNITTEST_ASSERT(pStrm->Position() == 0);
		pStrm->SetLength(0);
		UNITTEST_ASSERT(pStrm->Length() == 0);

		pStrm->Close();

		UNITTEST_ASSERT(File::Exists(sampleFile));
		UNITTEST_ASSERT(File::Delete(sampleFile));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestTextStream);

// -------------------------------------------------------------------------- //
#endif
