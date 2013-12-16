// -------------------------------------------------------------------------- //
//	文件名		：	test_textrw_read.cpp
//	创建者		：	杨钢
//	创建时间	：	4/30/2008 9:12:20 AM
//	功能描述	：	
//
// -------------------------------------------------------------------------- //

#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

struct _SequenceWritingBuffer : SequenceWritingBuffer
{
	typedef SequenceWritingBuffer base;
	using base::m_bufferSize;
	using base::m_pBuffer;
	using base::m_iob;
};

struct _StreamWriter : StreamWriter
{
	typedef StreamWriter base;
	using base::m_sqb;
};

// -------------------------------------------------------------------------- //

class TestTextWriter: public UnitTestSuite
{
	UNITTEST_SUITE(TestTextWriter)
		UNITTEST_METHOD(testBufferInit)
		UNITTEST_METHOD(testBufferEnabled)
		UNITTEST_METHOD(testBufferDisabled)
		UNITTEST_METHOD(testWriter)
		UNITTEST_METHOD(testFileWriter)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void tearDown()
	{
	}

	void testBufferInit()
	{
		ref<SequenceWritingBuffer> swb = gc_new<SequenceWritingBuffer>();
		ref<_SequenceWritingBuffer> _swb = ref_cast<_SequenceWritingBuffer>(swb);
		
		ref<Stream> os = gc_new<MemoryStream>();
		_swb->Open(os, 0);
		UNITTEST_ASSERT(_swb->m_bufferSize == 0 && _swb->m_pBuffer == NULL);
		
		for (int i = 1; i <= 8; ++i)
		{
			os = gc_new<MemoryStream>();
			_swb->Open(os, i);
			UNITTEST_ASSERT(_swb->m_bufferSize == 8 && _swb->m_pBuffer != NULL);
		}

		os = gc_new<MemoryStream>();
		_swb->Open(os, 9);
		UNITTEST_ASSERT(_swb->m_bufferSize == 9 && _swb->m_pBuffer != NULL);
	}

	void testBufferEnabled()
	{
		ref<MemoryStream> os = gc_new<MemoryStream>();
		ref<SequenceWritingBuffer> swb = gc_new<SequenceWritingBuffer>(os, 8);
		ref<_SequenceWritingBuffer> _swb = ref_cast<_SequenceWritingBuffer>(swb);
		
		UNITTEST_ASSERT(_swb->m_iob == 0 && os->Length() == 0);

		_swb->WriteByte(0x30);
		UNITTEST_ASSERT(_swb->m_iob == 1 && os->Length() == 0);

		_swb->WriteInt16(0x3231);
		UNITTEST_ASSERT(_swb->m_iob == 3 && os->Length() == 0);

		_swb->WriteInt32(0x36353433);
		UNITTEST_ASSERT(_swb->m_iob == 7 && os->Length() == 0);
		UNITTEST_ASSERT(memcmp(_swb->m_pBuffer, "0123456", 7) == 0);
		
		UINT value = 0x41393837;
		_swb->Write(&value, 4);
		UNITTEST_ASSERT(_swb->m_iob == 3 && os->Length() == 8);
		UNITTEST_ASSERT(strncmp((const char*)_swb->m_pBuffer, "89A", 3) == 0);
		UNITTEST_ASSERT(os->Length() == 8 && memcmp(os->GetRawBuffer(), "01234567", 8) == 0);

		_swb->Close();
	}

	void testBufferDisabled()
	{
		ref<MemoryStream> os = gc_new<MemoryStream>();
		ref<SequenceWritingBuffer> swb = gc_new<SequenceWritingBuffer>(os, 0);
		ref<_SequenceWritingBuffer> _swb = ref_cast<_SequenceWritingBuffer>(swb);
		
		UNITTEST_ASSERT(os->Length() == 0);

		_swb->WriteByte(0x30);
		UNITTEST_ASSERT(os->Length() == 1);

		_swb->WriteInt16(0x3231);
		UNITTEST_ASSERT(os->Length() == 3);

		_swb->WriteInt32(0x36353433);
		UNITTEST_ASSERT(os->Length() == 7);
		UNITTEST_ASSERT(memcmp(os->GetRawBuffer(), "0123456", 7) == 0);
		
		UINT value = 0x41393837;
		_swb->Write(&value, 4);
		UNITTEST_ASSERT(os->Length() == 11 && memcmp(os->GetRawBuffer(), "0123456789A", 11) == 0);

		_swb->Close();
	}

	void testWriter()
	{
		ref<MemoryStream> os = gc_new<MemoryStream>();
		ref<StreamWriter> tw = gc_new<StreamWriter>(os, Encoding::get_UTF8(), 8);
		ref<_StreamWriter> _tw = ref_cast<_StreamWriter>(tw);
		ref<_SequenceWritingBuffer> _sqb = ref_cast<_SequenceWritingBuffer>(_tw->m_sqb);

		tw->Write(TEXT_ChsEng_Wide);
		UNITTEST_ASSERT(_sqb->m_iob == 1 && memcmp(_sqb->m_pBuffer, TEXT_ChsEng_UTF8 + 8, 1) == 0);
		UNITTEST_ASSERT(os->Length() == 8 && memcmp(os->GetRawBuffer(), TEXT_ChsEng_UTF8, 8) == 0);

		tw->Flush();
		UNITTEST_ASSERT(_sqb->m_iob == 0);
		UNITTEST_ASSERT(os->Length() == 9 && memcmp(os->GetRawBuffer(), TEXT_ChsEng_UTF8, 9) == 0);

		tw->Close();
	}

	void testFileWriter()
	{
		stringx filename = __X("test-textwr.txt");
		ref<TextWriter> tw = gc_new<StreamWriter>(filename, Encoding::get_UTF8(), 8);
		tw->WriteLine(__X("Line 1"));
		tw->Write(TEXT_ChsEng_Wide);
		tw->WriteChar('\n');
		tw->Close();

		ref<TextReader> tr = gc_new<StreamReader>(filename, Encoding::get_UTF8());
		stringx line;
		UNITTEST_ASSERT((line = tr->ReadLine()) == __X("Line 1"));
		UNITTEST_ASSERT((line = tr->ReadLine()) == TEXT_ChsEng_Wide);
		UNITTEST_ASSERT((line = tr->ReadLine()) == NULL);
		tr->Close();
	}
};

UNITTEST_SUITE_REGISTRATION(TestTextWriter);

// -------------------------------------------------------------------------- //
#endif
