// -------------------------------------------------------------------------- //
//	�ļ���		��	test_textrw_read.cpp
//	������		��	���
//	����ʱ��	��	4/30/2008 9:12:20 AM
//	��������	��	
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

// -------------------------------------------------------------------------- //

class TestTextWriter: public UnitTestSuite
{
	UNITTEST_SUITE(TestTextWriter)
		UNITTEST_METHOD(testBufferInit)
		UNITTEST_METHOD(testBufferEnabled)
		UNITTEST_METHOD(testBufferDisabled)
#if 0
		UNITTEST_METHOD(testSqbAlignFile)
		UNITTEST_METHOD(testSqbOddFile)
		UNITTEST_METHOD(testSqbReadMethods)
		UNITTEST_METHOD(testOpenTextAnsi)
		UNITTEST_METHOD(testOpenTextUnicode)
		UNITTEST_METHOD(testOpenTextUnicodeNs)
		UNITTEST_METHOD(testOpenTextUtf8)
		UNITTEST_METHOD(testOpenTextUtf8Ns)
		UNITTEST_METHOD(testReadChar)
		UNITTEST_METHOD(testReset)
#endif
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
		UNITTEST_ASSERT(os->Length() == 11 && memcmp(os->GetRawBuffer(), "0123456789A", 8) == 0);

		_swb->Close();
	}

#if 0
	void testSqbAlignFile()
	{
		// ��ʼ������
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_temp.txt"));
		ref<Stream>	pStrm;
		MakeFillFile(sampleFile, pStrm, 65536); 
		ref<SequenceReadingBuffer>	sqb = gc_new<SequenceReadingBuffer>(pStrm);
		_SequenceReadingBuffer*	_sqb = (_SequenceReadingBuffer*)(sqb.__GetPointer()); 

		WORD*	buffer = (WORD*)malloc(16384);		// 16KB

		// ���Զ���С������
		UNITTEST_ASSERT(sqb->Read(buffer, 16) == 16);
		UNITTEST_ASSERT(buffer[0] == 0 && buffer[7] == 7);
		UNITTEST_ASSERT(sqb->Read(buffer, 16) == 16);
		UNITTEST_ASSERT(buffer[0] == 8 && buffer[7] == 15);
		UNITTEST_ASSERT(sqb->Pos() == 32);

		// ��λ���ӿ�ʼ��
		UNITTEST_ASSERT(sqb->Seek(5000, Stream::SeekBegin) == 5000);
		UNITTEST_ASSERT(_sqb->m_pos == 4096 && _sqb->m_eob == 4096 && _sqb->m_iob == 904);
		UNITTEST_ASSERT(sqb->Read(buffer, 16) == 16 && buffer[0] == 2500);

		// ��λ���ӵ�ǰλ�ã�������
		UNITTEST_ASSERT(sqb->Seek(-1016, Stream::SeekCurrent) == 4000);
		UNITTEST_ASSERT(_sqb->m_pos == 0 && _sqb->m_eob == 4096 && _sqb->m_iob == 4000);
		UNITTEST_ASSERT(sqb->Read(buffer, 512) == 512);
		UNITTEST_ASSERT(sqb->Pos() == 4512 && _sqb->m_pos == 4096 && _sqb->m_eob == 4096 && _sqb->m_iob == 416);
		UNITTEST_ASSERT(buffer[0] == 2000 && buffer[255] == 2255);

		// ��λ����β����������ĩβ
		UNITTEST_ASSERT(sqb->Seek(-32, Stream::SeekEnd) == 65504);
		UNITTEST_ASSERT(_sqb->m_pos == 61440 && _sqb->m_eob == 4096 && _sqb->m_iob == 4064);
		UNITTEST_ASSERT(sqb->Read(buffer, 64) == 32);
		UNITTEST_ASSERT(sqb->Pos() == 65536 && _sqb->m_pos == 65536 && _sqb->m_eob == 0 && _sqb->m_iob == 0);
		UNITTEST_ASSERT(buffer[0] == 32752 && buffer[15] == 32767);

		// ��ζ�
		UNITTEST_ASSERT(sqb->Seek(2048, Stream::SeekBegin) == 2048);
		UNITTEST_ASSERT(sqb->Read(buffer, 16384) == 16384);
		UNITTEST_ASSERT(sqb->Pos() == 18432 && buffer[0] == 1024 && buffer[8191] == 1024 + 8191);

		UNITTEST_ASSERT(sqb->Seek(51200, Stream::SeekBegin) == 51200);
		UNITTEST_ASSERT(sqb->Read(buffer, 16384) == 14336);
		UNITTEST_ASSERT(sqb->Pos() == 65536 && buffer[0] == 25600 && buffer[7167] == 32767);

		UNITTEST_ASSERT(sqb->Seek(-49152, Stream::SeekCurrent) == 16384);
		UNITTEST_ASSERT(sqb->Read(buffer, 16384) == 16384);
		UNITTEST_ASSERT(sqb->Pos() == 32768 && buffer[0] == 8192 && buffer[8191] == 16383);

		// ������
		free(buffer);
		sqb->Close();
		pStrm->Close();
		File::Delete(sampleFile);
	}

	void testSqbOddFile()
	{
		// ���䰸�����ļ����Ȳ��� 4K ��������

		// ��ʼ������
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_temp.txt"));
		ref<Stream>	pStrm;
		MakeFillFile(sampleFile, pStrm, 6000);
		ref<SequenceReadingBuffer>	sqb = gc_new<SequenceReadingBuffer>(pStrm);

		// ����Խ��������
		WORD	buffer[3000];
		UNITTEST_ASSERT(sqb->Seek(3000, Stream::SeekBegin) == 3000);
		UNITTEST_ASSERT(sqb->Read(buffer, 6000) == 3000);
		UNITTEST_ASSERT(sqb->Pos() == 6000 && buffer[0] == 1500 && buffer[1499] == 2999);

		// ������
		sqb->Close();
		pStrm->Close();
		File::Delete(sampleFile);
	}

	void testSqbReadMethods()
	{
		// ���� ReadByte / ReadInt16 / ReadInt32 ����

		// ��ʼ������
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_temp.txt"));
		ref<Stream>	pStrm;
		MakeFillFile(sampleFile, pStrm, 6000);
		ref<SequenceReadingBuffer>	sqb = gc_new<SequenceReadingBuffer>(pStrm);
		_SequenceReadingBuffer*	_sqb = (_SequenceReadingBuffer*)(sqb.__GetPointer());

		// ����Ķ�ȡ
		sqb->Seek(8, Stream::SeekBegin);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x00050004 && sqb->Pos() == 12);
		UNITTEST_ASSERT(sqb->ReadInt16() == 6 && sqb->Pos() == 14);
		UNITTEST_ASSERT(sqb->ReadByte() == 7 && sqb->Pos() == 15);

		// ��ȡ���¿��
		sqb->Seek(4092, Stream::SeekBegin);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x07FF07FE && sqb->Pos() == 4096 && _sqb->m_eob == 1904);
		sqb->Seek(-2, Stream::SeekCurrent);
		UNITTEST_ASSERT(sqb->ReadInt16() == 0x7FF && sqb->Pos() == 4096 && _sqb->m_eob == 1904);
		sqb->Seek(-1, Stream::SeekCurrent);
		UNITTEST_ASSERT(sqb->ReadByte() == 7 && sqb->Pos() == 4096 && _sqb->m_eob == 1904);

		// �Ƕ��������
		sqb->Seek(8 + 1, Stream::SeekBegin);
		UNITTEST_ASSERT(sqb->ReadInt16() == 0x0500);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x07000600);
		sqb->Seek(8 + 2, Stream::SeekBegin);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x00060005);

		// �����Խ��
		sqb->Seek(-1, Stream::SeekEnd);
		UNITTEST_ASSERT(sqb->ReadByte() == 0xB);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadByte());
		sqb->Seek(-2, Stream::SeekEnd);
		UNITTEST_ASSERT(sqb->ReadInt16() == 0x0BB7);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadInt16());
		sqb->Seek(-4, Stream::SeekEnd);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x0BB70BB6);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadInt32());

		// �Ƕ����Խ��
		sqb->Seek(-1, Stream::SeekEnd);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadInt16());
		sqb->Seek(-3, Stream::SeekEnd);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadInt32());

		// ������
		sqb->Close();
		pStrm->Close();
		File::Delete(sampleFile);
	}

	void testOpenTextAnsi()
	{
		// ָ�������
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<TextReader> pReader = gc_new<TextReader>(sampleFile, gc_new<Encoding>(936));

		stringx s;
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Hello, KFC"));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChsLine2);
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X(""));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("END"));
		UNITTEST_ASSERT(!pReader->ReadLine(s));

		pReader->Close();

		// ���´�һ�Σ���ָ������
		pReader = gc_new<TextReader>(sampleFile);
		_TextReader* _pReader = (_TextReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_Default() ));

		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Hello, KFC"));
#ifdef X_OS_WINDOWS
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChsLine2);
#endif

		pReader->Close();

		// ָ���������ᵼ���쳣
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<TextReader>(sampleFile, Encoding::get_Unicode()));
	}

	void testOpenTextUnicode()
	{
		// ָ�������
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_unicode.txt"));
		ref<TextReader> pReader = gc_new<TextReader>(sampleFile, Encoding::get_Unicode());

		stringx s;
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Encoding: Unicode"));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChs);
		UNITTEST_ASSERT(!pReader->ReadLine(s));

		pReader->Close();

		// ��ָ�������
		pReader = gc_new<TextReader>(sampleFile);
		_TextReader* _pReader = (_TextReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_Unicode() ));

		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Encoding: Unicode"));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChs);

		pReader->Close();

		// ָ���������ᵼ���쳣
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<TextReader>(sampleFile, Encoding::get_UTF8()));
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<TextReader>(sampleFile, Encoding::get_Default()));
	}

	void testOpenTextUnicodeNs()
	{
		// ������������� testOpenTextUnicode һ����ֻ�������ļ�û�� FF FE ǩ��
		// �������ļ��Ƿǳ�����׼�ģ����Ǻܲ��ң�ȷʵ���������ļ�

		// ָ�������
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_unicode_ns.txt"));
		ref<TextReader> pReader = gc_new<TextReader>(sampleFile, Encoding::get_Unicode());

		stringx s;
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Encoding: Unicode"));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChs);
		UNITTEST_ASSERT(!pReader->ReadLine(s));

		pReader->Close();

		// ��ָ�������
		pReader = gc_new<TextReader>(sampleFile);
		_TextReader* _pReader = (_TextReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_Unicode() ));

		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Encoding: Unicode"));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChs);

		pReader->Close();

		// ָ���������ᵼ���쳣
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<TextReader>(sampleFile, Encoding::get_UTF8()));
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<TextReader>(sampleFile, Encoding::get_Default()));
	}

	void testOpenTextUtf8()
	{
		// ָ�������
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_utf8.txt"));
		ref<TextReader> pReader = gc_new<TextReader>(sampleFile, Encoding::get_UTF8());

		stringx s;
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Encoding: UTF-8"));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChs);
		UNITTEST_ASSERT(!pReader->ReadLine(s));

		pReader->Close();

		// ���´�һ�Σ���ָ������
		pReader = gc_new<TextReader>(sampleFile);
		_TextReader* _pReader = (_TextReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_UTF8() ));

		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Encoding: UTF-8"));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChs);

		pReader->Close();

		// ָ���������ᵼ���쳣
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<TextReader>(sampleFile, Encoding::get_Default()));
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<TextReader>(sampleFile, Encoding::get_Unicode()));
	}

	void testOpenTextUtf8Ns()
	{
		// ָ�������
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_utf8_ns.txt"));
		ref<TextReader> pReader = gc_new<TextReader>(sampleFile, Encoding::get_UTF8());

		stringx s;
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Encoding: UTF-8"));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChs);
		UNITTEST_ASSERT(!pReader->ReadLine(s));

		pReader->Close();

		// ���´�һ�Σ���ָ������
		pReader = gc_new<TextReader>(sampleFile);
		_TextReader* _pReader = (_TextReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_Default() ));
		pReader->ChangeEncoding(Encoding::get_UTF8());

		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == __X("Encoding: UTF-8"));
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(s == pwszChs);

		pReader->Close();

		// ָ���������ᵼ���쳣
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<TextReader>(sampleFile, Encoding::get_Unicode()));
	}

	void testReadChar()
	{
		// MBCS
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<TextReader> pReader = gc_new<TextReader>(sampleFile, gc_new<Encoding>(936));

		stringx s;
		UNITTEST_ASSERT(pReader->ReadChar() == 'H');
		UNITTEST_ASSERT(pReader->ReadChar() == 'e');
		UNITTEST_ASSERT(pReader->ReadChar() == 'l');
		UNITTEST_ASSERT(pReader->ReadLine(s));
		UNITTEST_ASSERT(pReader->ReadChar() == pwszChs[0]);
		UNITTEST_ASSERT(pReader->ReadChar() == pwszChs[1]);

		pReader->Close();

		// Unicode
		sampleFile = TestEnv::get_sample_path(__X("txtr_samp_unicode.txt"));
		pReader = gc_new<TextReader>(sampleFile, Encoding::get_Unicode());

		UNITTEST_ASSERT(pReader->ReadChar() == 'E');
		UNITTEST_ASSERT(pReader->ReadChar() == 'n');
		UNITTEST_ASSERT(pReader->ReadLine(s));		
		UNITTEST_ASSERT(pReader->ReadChar() == pwszChs[0]);
		UNITTEST_ASSERT(pReader->ReadChar() == pwszChs[1]);

		pReader->Close();

		// UTF-8
		sampleFile = TestEnv::get_sample_path(__X("txtr_samp_utf8.txt"));
		pReader = gc_new<TextReader>(sampleFile, Encoding::get_UTF8());

		UNITTEST_ASSERT(pReader->ReadChar() == 'E');
		UNITTEST_ASSERT(pReader->ReadChar() == 'n');
		UNITTEST_ASSERT(pReader->ReadLine(s));		
		UNITTEST_ASSERT(pReader->ReadChar() == pwszChs[0]);
		UNITTEST_ASSERT(pReader->ReadChar() == pwszChs[1]);

		pReader->Close();
	}

	void testReset()
	{
		// ���� Reset ����

		// ANSI
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<TextReader> pReader = gc_new<TextReader>(sampleFile, Encoding::get_Default());
		_TextReader* _pReader = (_TextReader*)(pReader.__GetPointer());
		pReader->ReadChar();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 1);		// 0 �ֽ�ͷ + һ�� char �ַ�
		pReader->Reset();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 0);
		pReader->Close();

		sampleFile = TestEnv::get_sample_path(__X("txtr_samp_unicode.txt"));
		pReader = gc_new<TextReader>(sampleFile);
		_pReader = (_TextReader*)(pReader.__GetPointer());
		pReader->ReadChar();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 4);		// 2 �ֽ�ͷ + һ�� WIDE �ַ�
		pReader->Reset();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 2);
		pReader->Close();

		sampleFile = TestEnv::get_sample_path(__X("txtr_samp_utf8.txt"));
		pReader = gc_new<TextReader>(sampleFile, Encoding::get_UTF8());
		_pReader = (_TextReader*)(pReader.__GetPointer());
		pReader->ReadChar();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 4);		// 3 �ֽ�ͷ + һ�� char �ַ�
		pReader->Reset();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 3);
		pReader->Close();
	}

private:
	void MakeFillFile(stringx sampleFile, ref<Stream>& ppStream, UINT len)
	{
		// �����ļ�
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessReadWrite, true);
		ppStream = pStrm;

		// Ԥ�ȷ���ռ�
		// pStrm->Seek(len, Stream::SeekBegin);
		pStrm->SetLength(len);
		pStrm->Seek(0, Stream::SeekBegin);

		// д������
		ASSERT(len < 65536 * 2);	// WORD ����ֻ��֧�� 65536 ��Ԫ��
		UINT	cbTotal = len;
		WORD	buffer[256];		// һ������
		WORD	sk = 0;
		while (cbTotal > 0)
		{
			for (UINT i = 0; i < 256; ++i)
				buffer[i] = sk++;
			UINT cbToWrite = std::min(cbTotal, 512U);
			UNITTEST_ASSERT(pStrm->Write(buffer, cbToWrite) == cbToWrite);
			cbTotal -= cbToWrite;
		}
		UNITTEST_ASSERT(pStrm->Length() == len && cbTotal == 0);
	}
#endif
};

UNITTEST_SUITE_REGISTRATION(TestTextWriter);

// -------------------------------------------------------------------------- //
#endif
