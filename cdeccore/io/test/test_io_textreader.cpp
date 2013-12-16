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

struct _SequenceReadingBuffer: SequenceReadingBuffer
{
	typedef SequenceReadingBuffer base;
	using base::m_pos;
	using base::m_iob;
	using base::m_eob;
};

struct _StreamReader: StreamReader
{
	typedef StreamReader base;
	using base::m_encoding;
	using base::m_sqb;
};

class TestStreamReader: public UnitTestSuite
{
	UNITTEST_SUITE(TestStreamReader)
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
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testSqbAlignFile()
	{
		// 初始化环境
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_temp.txt"));
		ref<Stream>	pStrm;
		MakeFillFile(sampleFile, pStrm, 65536); 
		ref<SequenceReadingBuffer>	sqb = gc_new<SequenceReadingBuffer>(pStrm);
		_SequenceReadingBuffer*	_sqb = (_SequenceReadingBuffer*)(sqb.__GetPointer()); 

		WORD*	buffer = (WORD*)malloc(16384);		// 16KB

		// 测试读入小块数据
		UNITTEST_ASSERT(sqb->Read(buffer, 16) == 16);
		UNITTEST_ASSERT(buffer[0] == 0 && buffer[7] == 7);
		UNITTEST_ASSERT(sqb->Read(buffer, 16) == 16);
		UNITTEST_ASSERT(buffer[0] == 8 && buffer[7] == 15);
		UNITTEST_ASSERT(sqb->Pos() == 32);

		// 定位（从开始）
		UNITTEST_ASSERT(sqb->Seek(5000, Stream::SeekBegin) == 5000);
		UNITTEST_ASSERT(_sqb->m_pos == 4096 && _sqb->m_eob == 4096 && _sqb->m_iob == 904);
		UNITTEST_ASSERT(sqb->Read(buffer, 16) == 16 && buffer[0] == 2500);

		// 定位（从当前位置），跨块读
		UNITTEST_ASSERT(sqb->Seek(-1016, Stream::SeekCurrent) == 4000);
		UNITTEST_ASSERT(_sqb->m_pos == 0 && _sqb->m_eob == 4096 && _sqb->m_iob == 4000);
		UNITTEST_ASSERT(sqb->Read(buffer, 512) == 512);
		UNITTEST_ASSERT(sqb->Pos() == 4512 && _sqb->m_pos == 4096 && _sqb->m_eob == 4096 && _sqb->m_iob == 416);
		UNITTEST_ASSERT(buffer[0] == 2000 && buffer[255] == 2255);

		// 定位（从尾部），读到末尾
		UNITTEST_ASSERT(sqb->Seek(-32, Stream::SeekEnd) == 65504);
		UNITTEST_ASSERT(_sqb->m_pos == 61440 && _sqb->m_eob == 4096 && _sqb->m_iob == 4064);
		UNITTEST_ASSERT(sqb->Read(buffer, 64) == 32);
		UNITTEST_ASSERT(sqb->Pos() == 65536 && _sqb->m_pos == 65536 && _sqb->m_eob == 0 && _sqb->m_iob == 0);
		UNITTEST_ASSERT(buffer[0] == 32752 && buffer[15] == 32767);

		// 大段读
		UNITTEST_ASSERT(sqb->Seek(2048, Stream::SeekBegin) == 2048);
		UNITTEST_ASSERT(sqb->Read(buffer, 16384) == 16384);
		UNITTEST_ASSERT(sqb->Pos() == 18432 && buffer[0] == 1024 && buffer[8191] == 1024 + 8191);

		UNITTEST_ASSERT(sqb->Seek(51200, Stream::SeekBegin) == 51200);
		UNITTEST_ASSERT(sqb->Read(buffer, 16384) == 14336);
		UNITTEST_ASSERT(sqb->Pos() == 65536 && buffer[0] == 25600 && buffer[7167] == 32767);

		UNITTEST_ASSERT(sqb->Seek(-49152, Stream::SeekCurrent) == 16384);
		UNITTEST_ASSERT(sqb->Read(buffer, 16384) == 16384);
		UNITTEST_ASSERT(sqb->Pos() == 32768 && buffer[0] == 8192 && buffer[8191] == 16383);

		// 清理工作
		free(buffer);
		sqb->Close();
		pStrm->Close();
		File::Delete(sampleFile);
	}

	void testSqbOddFile()
	{
		// 补充案例，文件长度不是 4K 的整数倍

		// 初始化环境
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_temp.txt"));
		ref<Stream>	pStrm;
		MakeFillFile(sampleFile, pStrm, 6000);
		ref<SequenceReadingBuffer>	sqb = gc_new<SequenceReadingBuffer>(pStrm);

		// 读入越过的数据
		WORD	buffer[3000];
		UNITTEST_ASSERT(sqb->Seek(3000, Stream::SeekBegin) == 3000);
		UNITTEST_ASSERT(sqb->Read(buffer, 6000) == 3000);
		UNITTEST_ASSERT(sqb->Pos() == 6000 && buffer[0] == 1500 && buffer[1499] == 2999);

		// 清理工作
		sqb->Close();
		pStrm->Close();
		File::Delete(sampleFile);
	}

	void testSqbReadMethods()
	{
		// 测试 ReadByte / ReadInt16 / ReadInt32 方法

		// 初始化环境
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_temp.txt"));
		ref<Stream>	pStrm;
		MakeFillFile(sampleFile, pStrm, 6000);
		ref<SequenceReadingBuffer>	sqb = gc_new<SequenceReadingBuffer>(pStrm);
		_SequenceReadingBuffer*	_sqb = (_SequenceReadingBuffer*)(sqb.__GetPointer());

		// 对齐的读取
		sqb->Seek(8, Stream::SeekBegin);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x00050004 && sqb->Pos() == 12);
		UNITTEST_ASSERT(sqb->ReadInt16() == 6 && sqb->Pos() == 14);
		UNITTEST_ASSERT(sqb->ReadByte() == 7 && sqb->Pos() == 15);

		// 读取导致跨界
		sqb->Seek(4092, Stream::SeekBegin);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x07FF07FE && sqb->Pos() == 4096 && _sqb->m_eob == 1904);
		sqb->Seek(-2, Stream::SeekCurrent);
		UNITTEST_ASSERT(sqb->ReadInt16() == 0x7FF && sqb->Pos() == 4096 && _sqb->m_eob == 1904);
		sqb->Seek(-1, Stream::SeekCurrent);
		UNITTEST_ASSERT(sqb->ReadByte() == 7 && sqb->Pos() == 4096 && _sqb->m_eob == 1904);

		// 非对齐读操作
		sqb->Seek(8 + 1, Stream::SeekBegin);
		UNITTEST_ASSERT(sqb->ReadInt16() == 0x0500);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x07000600);
		sqb->Seek(8 + 2, Stream::SeekBegin);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x00060005);

		// 对齐读越界
		sqb->Seek(-1, Stream::SeekEnd);
		UNITTEST_ASSERT(sqb->ReadByte() == 0xB);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadByte());
		sqb->Seek(-2, Stream::SeekEnd);
		UNITTEST_ASSERT(sqb->ReadInt16() == 0x0BB7);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadInt16());
		sqb->Seek(-4, Stream::SeekEnd);
		UNITTEST_ASSERT(sqb->ReadInt32() == 0x0BB70BB6);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadInt32());

		// 非对齐读越界
		sqb->Seek(-1, Stream::SeekEnd);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadInt16());
		sqb->Seek(-3, Stream::SeekEnd);
		UNITTEST_ASSERT_EXCEPTION(sqb->ReadInt32());

		// 清理工作
		sqb->Close();
		pStrm->Close();
		File::Delete(sampleFile);
	}

	void testOpenTextAnsi()
	{
		// 指定编码打开
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<StreamReader> pReader = gc_new<StreamReader>(sampleFile, gc_new<Encoding>(936));

		stringx s;
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Hello, KFC"));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_ChsLine2);
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X(""));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("END"));
		UNITTEST_ASSERT(pReader->ReadLine() == NULL);

		pReader->Close();

		// 重新打开一次，不指定编码
		pReader = gc_new<StreamReader>(sampleFile);
		_StreamReader* _pReader = (_StreamReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_Default() ));

		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Hello, KFC"));
#ifdef X_OS_WINDOWS
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_ChsLine2);
#endif

		pReader->Close();

		// 指定错误编码会导致异常
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<StreamReader>(sampleFile, Encoding::get_Unicode()));
	}

	void testOpenTextUnicode()
	{
		// 指定编码打开
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_unicode.txt"));
		ref<StreamReader> pReader = gc_new<StreamReader>(sampleFile, Encoding::get_Unicode());

		stringx s;
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Encoding: Unicode"));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_Chs);
		UNITTEST_ASSERT(pReader->ReadLine() == NULL);

		pReader->Close();

		// 不指定编码打开
		pReader = gc_new<StreamReader>(sampleFile);
		_StreamReader* _pReader = (_StreamReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_Unicode() ));

		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Encoding: Unicode"));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_Chs);

		pReader->Close();

		// 指定错误编码会导致异常
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<StreamReader>(sampleFile, Encoding::get_UTF8()));
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<StreamReader>(sampleFile, Encoding::get_Default()));
	}

	void testOpenTextUnicodeNs()
	{
		// 这个案例几乎与 testOpenTextUnicode 一样，只是样张文件没有 FF FE 签名
		// 这样的文件是非常不标准的，但是很不幸，确实有这样的文件

		// 指定编码打开
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_unicode_ns.txt"));
		ref<StreamReader> pReader = gc_new<StreamReader>(sampleFile, Encoding::get_Unicode());

		stringx s;
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Encoding: Unicode"));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_Chs);
		UNITTEST_ASSERT(pReader->ReadLine() == NULL);

		pReader->Close();

		// 不指定编码打开
		pReader = gc_new<StreamReader>(sampleFile);
		_StreamReader* _pReader = (_StreamReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_Unicode() ));

		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Encoding: Unicode"));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_Chs);

		pReader->Close();

		// 指定错误编码会导致异常
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<StreamReader>(sampleFile, Encoding::get_UTF8()));
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<StreamReader>(sampleFile, Encoding::get_Default()));
	}

	void testOpenTextUtf8()
	{
		// 指定编码打开
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_utf8.txt"));
		ref<StreamReader> pReader = gc_new<StreamReader>(sampleFile, Encoding::get_UTF8());

		stringx s;
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Encoding: UTF-8"));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_Chs);
		UNITTEST_ASSERT(pReader->ReadLine() == NULL);

		pReader->Close();

		// 重新打开一次，不指定编码
		pReader = gc_new<StreamReader>(sampleFile);
		_StreamReader* _pReader = (_StreamReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_UTF8() ));

		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Encoding: UTF-8"));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_Chs);

		pReader->Close();

		// 指定错误编码会导致异常
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<StreamReader>(sampleFile, Encoding::get_Default()));
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<StreamReader>(sampleFile, Encoding::get_Unicode()));
	}

	void testOpenTextUtf8Ns()
	{
		// 指定编码打开
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_utf8_ns.txt"));
		ref<StreamReader> pReader = gc_new<StreamReader>(sampleFile, Encoding::get_UTF8());

		stringx s;
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Encoding: UTF-8"));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_Chs);
		UNITTEST_ASSERT(pReader->ReadLine() == NULL);

		pReader->Close();

		// 重新打开一次，不指定编码
		pReader = gc_new<StreamReader>(sampleFile);
		_StreamReader* _pReader = (_StreamReader*)(pReader.__GetPointer());
		UNITTEST_ASSERT(_pReader->m_encoding->IsEqual( Encoding::get_Default() ));
		pReader->ChangeEncoding(Encoding::get_UTF8());

		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == __X("Encoding: UTF-8"));
		s = pReader->ReadLine();
		UNITTEST_ASSERT(s == TEXT_Chs);

		pReader->Close();

		// 指定错误编码会导致异常
		// TODO pure virtual function call
	//	UNITTEST_ASSERT_EXCEPTION(gc_new<StreamReader>(sampleFile, Encoding::get_Unicode()));
	}

	void testReadChar()
	{
		// MBCS
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<StreamReader> pReader = gc_new<StreamReader>(sampleFile, gc_new<Encoding>(936));

		stringx s;
		UNITTEST_ASSERT(pReader->ReadChar() == 'H');
		UNITTEST_ASSERT(pReader->ReadChar() == 'e');
		UNITTEST_ASSERT(pReader->ReadChar() == 'l');
		s = pReader->ReadLine();
		UNITTEST_ASSERT(pReader->ReadChar() == TEXT_Chs[0]);
		UNITTEST_ASSERT(pReader->ReadChar() == TEXT_Chs[1]);

		pReader->Close();

		// Unicode
		sampleFile = TestEnv::get_sample_path(__X("txtr_samp_unicode.txt"));
		pReader = gc_new<StreamReader>(sampleFile, Encoding::get_Unicode());

		UNITTEST_ASSERT(pReader->ReadChar() == 'E');
		UNITTEST_ASSERT(pReader->ReadChar() == 'n');
		s = pReader->ReadLine();		
		UNITTEST_ASSERT(pReader->ReadChar() == TEXT_Chs[0]);
		UNITTEST_ASSERT(pReader->ReadChar() == TEXT_Chs[1]);

		pReader->Close();

		// UTF-8
		sampleFile = TestEnv::get_sample_path(__X("txtr_samp_utf8.txt"));
		pReader = gc_new<StreamReader>(sampleFile, Encoding::get_UTF8());

		UNITTEST_ASSERT(pReader->ReadChar() == 'E');
		UNITTEST_ASSERT(pReader->ReadChar() == 'n');
		s = pReader->ReadLine();		
		UNITTEST_ASSERT(pReader->ReadChar() == TEXT_Chs[0]);
		UNITTEST_ASSERT(pReader->ReadChar() == TEXT_Chs[1]);

		pReader->Close();
	}

	void testReset()
	{
		// 测试 Reset 方法

		// ANSI
		stringx sampleFile = TestEnv::get_sample_path(__X("txtr_samp_1.txt"));
		ref<StreamReader> pReader = gc_new<StreamReader>(sampleFile, Encoding::get_Default());
		_StreamReader* _pReader = (_StreamReader*)(pReader.__GetPointer());
		pReader->ReadChar();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 1);		// 0 字节头 + 一个 char 字符
		pReader->Reset();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 0);
		pReader->Close();

		sampleFile = TestEnv::get_sample_path(__X("txtr_samp_unicode.txt"));
		pReader = gc_new<StreamReader>(sampleFile);
		_pReader = (_StreamReader*)(pReader.__GetPointer());
		pReader->ReadChar();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 4);		// 2 字节头 + 一个 WIDE 字符
		pReader->Reset();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 2);
		pReader->Close();

		sampleFile = TestEnv::get_sample_path(__X("txtr_samp_utf8.txt"));
		pReader = gc_new<StreamReader>(sampleFile, Encoding::get_UTF8());
		_pReader = (_StreamReader*)(pReader.__GetPointer());
		pReader->ReadChar();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 4);		// 3 字节头 + 一个 char 字符
		pReader->Reset();
		UNITTEST_ASSERT(_pReader->m_sqb->Pos() == 3);
		pReader->Close();
	}

	void tearDown()
	{
	}

private:
	void MakeFillFile(stringx sampleFile, ref<Stream>& ppStream, UINT len)
	{
		// 创建文件
		ref<FileStream> pStrm = gc_new<FileStream>(sampleFile, FileStream::AccessReadWrite, true);
		ppStream = pStrm;

		// 预先分配空间
		// pStrm->Seek(len, Stream::SeekBegin);
		pStrm->SetLength(len);
		pStrm->Seek(0, Stream::SeekBegin);

		// 写入内容
		ASSERT(len < 65536 * 2);	// WORD 序列只能支持 65536 个元素
		UINT	cbTotal = len;
		WORD	buffer[256];		// 一个扇区
		WORD	sk = 0;
		while (cbTotal > 0)
		{
			for (UINT i = 0; i < 256; ++i)
				buffer[i] = sk++;
			UINT cbToWrite = Math::Min(cbTotal, 512U);
			UNITTEST_ASSERT(pStrm->Write(buffer, cbToWrite) == cbToWrite);
			cbTotal -= cbToWrite;
		}
		UNITTEST_ASSERT(pStrm->Length() == len && cbTotal == 0);
	}
};

UNITTEST_SUITE_REGISTRATION(TestStreamReader);

// -------------------------------------------------------------------------- //
#endif
