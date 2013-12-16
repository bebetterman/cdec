#include "stdafx.h"
//#include "textrw_stream.h"
//#include "textrw_reader.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

SequenceReadingBuffer::SequenceReadingBuffer():
m_pBuffer(NULL), m_pos(-1), m_eob(0), m_iob(0)
{
}

SequenceReadingBuffer::SequenceReadingBuffer(ref<Stream> pStream):
m_pBuffer(NULL), m_pos(-1), m_iob(0), m_eob(0)
{
	Open(pStream);
}

void SequenceReadingBuffer::Open(ref<Stream> pStream)
{
	Close();

	m_pStream = pStream;
	m_pBuffer = (BYTE*)malloc(BufferSize);

	ValidateBuffer(0);	// 当前 pos 处于无效的位置（-1）
}

void SequenceReadingBuffer::Close()
{
	m_pos = -1;
	m_iob = m_eob = 0;
}

void SequenceReadingBuffer::ValidateBuffer(INT64 pos)
{
	ASSERT(pos >= 0 && pos <= m_pStream->Length());
	const UINT BufferMask = BufferSize - 1;
	INT64 pos_n = pos;
	(UINT&)pos_n &= ~BufferMask;
	UINT iob_n = (UINT&)pos & BufferMask;

	if (pos_n != m_pos)
	{
		m_pos = pos_n;
		if (m_pStream->Position() != pos_n)
			m_pStream->Seek(pos_n, Stream::SeekBegin);
		ASSERT(m_pStream->Position() == m_pos);

		m_eob = m_pStream->Read(m_pBuffer, BufferSize);
	}
	m_iob = iob_n;
	ASSERT(m_iob < m_eob || (m_iob == m_eob && m_pos + m_eob == m_pStream->Length()));
}

// Patch: Windows.h defined a macro named min
#undef min

UINT SequenceReadingBuffer::Read(void* pv, UINT cb)
{
	BYTE*	pReceive = (BYTE*)pv;
	UINT	cbRead = 0;
	do
	{
		UINT	cbQuest = std::min(cb, m_eob - m_iob);
		if (cbQuest == 0)
		{
			ASSERT(Pos() == Size());
			break;
		}

		memcpy(pReceive, m_pBuffer + m_iob, cbQuest);
		pReceive += cbQuest;
		cbRead += cbQuest;
		cb -= cbQuest;
		m_iob += cbQuest;

		ASSERT(m_iob <= m_eob);
		if (m_iob >= m_eob)
			ValidateBuffer(m_pos + m_iob);
	} while(cb > 0);
	ASSERT(cb == 0 || Pos() == Size());
	return cbRead;
}

INT64 SequenceReadingBuffer::Seek(INT64 pos, Stream::SeekPosition cp)
{
	switch (cp)
	{
	case Stream::SeekCurrent:
		pos += Pos();
		break;
	case Stream::SeekEnd:
		pos += Size();
		break;
	default:
		ASSERT(cp == Stream::SeekBegin);
		break;
	}
	ASSERT(pos >= 0 && pos <= Size());
	ValidateBuffer(pos);
	ASSERT(Pos() == pos);
	return pos;
}

inline void SequenceReadingBuffer::PeekValue(DWORD* pValue, UINT typeSize)
{
	// 判断是否对齐
	// 对于字节（typeSize 为 1），此条件不可能满足
	if ((m_iob & (typeSize - 1)) != 0)
	{
		if (Read(pValue, typeSize) != typeSize)
			cdec_throw(IOException(EC_IO_EndOfStream));
		return;
	}

	// 判断是否越界
	if (m_iob + typeSize > m_eob)
		cdec_throw(IOException(EC_IO_EndOfStream));

	// 直接放入数据，此时缓冲区肯定足够
	switch (typeSize)
	{
	case 1:
		*(BYTE*)pValue = *(BYTE*)(m_pBuffer + m_iob);
		break;
	case 2:
		*(WORD*)pValue = *(WORD*)(m_pBuffer + m_iob);
		break;
	default:
		ASSERT(typeSize == 4);
		*pValue = *(DWORD*)(m_pBuffer + m_iob);
		break;
	}

	// 调整游标
	m_iob += typeSize;
	if (m_iob >= m_eob)
		ValidateBuffer(m_pos + m_iob);
}

BYTE SequenceReadingBuffer::ReadByte()
{
	DWORD	r;
	PeekValue(&r, sizeof(BYTE));
	return static_cast<BYTE>(r);
}

WORD SequenceReadingBuffer::ReadInt16()
{
	DWORD	r;
	PeekValue(&r, sizeof(WORD));
	return static_cast<WORD>(r);
}

DWORD SequenceReadingBuffer::ReadInt32()
{
	DWORD	r;
	PeekValue(&r, sizeof(DWORD));
	return r;
}

// -------------------------------------------------------------------------- //

StreamReader::StreamReader(stringx filename)
{
	ref<FileStream> pStream = gc_new<FileStream>(filename, FileStream::AccessRead, false);
	m_sqb = gc_new<SequenceReadingBuffer>();
	OpenNoEncoding(pStream);
}

StreamReader::StreamReader(stringx filename, ref<Encoding> encoding)
{
	ref<FileStream> pStream = gc_new<FileStream>(filename, FileStream::AccessRead, false);
	m_sqb = gc_new<SequenceReadingBuffer>();
	OpenEncoding(pStream, encoding);
}

UINT StreamReader::DetectCP()
{
	UINT	cp = 0;		// 0 表示系统默认 ANSI 代码页
	DWORD	dwMark = m_sqb->ReadInt32();

	m_posText = 0;
	if ((WORD&)dwMark == 0xFEFF)
	{
		cp = Encoding::UTF16;
		m_posText = 2;
	}
	else if ((WORD&)dwMark == 0xFFFE)
	{
		cp = Encoding::UTF16BE;
		m_posText = 2;
	}
	else if ((dwMark & 0xFFFFFF) == 0xBFBBEF)
	{
		cp = Encoding::UTF8;
		m_posText = 3;
	}
	else if ((dwMark & 0xFF00FF00) == 0)
	{
		cp = Encoding::UTF16;
	}

	m_sqb->Seek(m_posText, Stream::SeekBegin);
	return cp;
}

void StreamReader::OpenNoEncoding(ref<Stream> pStream)
{
	m_sqb->Open(pStream);

	// 判断流的编码属性，这里不进行复杂的猜测
	UINT	cp = DetectCP();
	if (cp != 0)
		m_encoding = gc_new<Encoding>(cp);
	else
		m_encoding = Encoding::get_Default();
}

void StreamReader::OpenEncoding(ref<Stream> pStream, ref<Encoding> encoding)
{
	m_sqb->Open(pStream);
	m_encoding = encoding;

	// 判断流的编码属性，这里不进行复杂的猜测
	UINT	cp = DetectCP();
	if (cp != 0 && cp != encoding->get_CP())
		cdec_throw(IOException(EC_InvalidArg));
	else if (cp == 0 && encoding->IsWideChar())
		cdec_throw(IOException(EC_InvalidArg));
}

void StreamReader::Close()
{
	m_sqb->Close();
}

stringx StreamReader::ReadLine()
{
	if (m_encoding->IsWideChar())
	{
		std::wstring16 _ws;
		bool f = FetchLineWideChars(_ws);
		ASSERT(f || IsEnd());
		return f ? stringx(_ws) : NULL;
	}
	else
	{
		std::string ss;
		bool f = FetchLineMultiBytes(ss);
		ASSERT(f || IsEnd());
		return f ? m_encoding->ToUnicode(ss) : NULL;
	}
}

bool StreamReader::FetchLineMultiBytes(std::string& s)
{
	ASSERT(s.empty());
	if (m_sqb->IsEnd())
		return false;

	if (s.capacity() < 32)
		s.reserve(32);	// 避免频繁地分配
	do
	{
		char ch = m_sqb->ReadByte();
		if (ch == 0xD)
		{
			// 后面可能有 0xA
			if (!m_sqb->IsEnd())
			{
				if (m_sqb->ReadByte() != 0xA)
					m_sqb->Seek(-1, Stream::SeekCurrent);
			}
			break;
		}
		else if (ch == 0xA)
			break;
		else
			s += ch;
	} while(!m_sqb->IsEnd());
	return true;
}

bool StreamReader::FetchLineWideChars(std::wstring16& s)
{
	ASSERT(s.empty());
	if (m_sqb->IsEnd())
		return false;

	if (s.capacity() < 32)
		s.reserve(32);	// 避免频繁地分配
	do
	{
		WCHAR ch = m_sqb->ReadInt16();
		if (ch == 0xD)
		{
			// 后面可能有 0xA
			if (!m_sqb->IsEnd())
			{
				if (m_sqb->ReadInt16() != 0xA)
					m_sqb->Seek(-2, Stream::SeekCurrent);
			}
			break;
		}
		else if (ch == 0xA)
			break;
		else
			s += ch;
	} while(!m_sqb->IsEnd());
	return true;
}

WCHAR StreamReader::ReadCharMultiBytes()
{
	// 缓存下来效率更高
	Encoding::CodePageClass cpc = m_encoding->get_Class();
	char ch[6];	// UTF8 最长
	ch[0] = m_sqb->ReadByte();

	switch (cpc)
	{
	case Encoding::SBCS:
		// 注意：默认设置下，char 为有符号数，其直接转为 WCHAR 时，会扩展符号位
		// 这导致例如 0xAA 转换为 0xFFAA。因此，必须首先转为 BYTE。
		return (WCHAR)(BYTE)ch[0];
	case Encoding::DBCS:
		if (m_encoding->IsDBCSLeading(ch[0]))
		{
			ch[1] = m_sqb->ReadByte();
			stringx ss = m_encoding->ToUnicode(ch, 2);
			ASSERT(ss.Length() == 1);
			return ss[0];
		}
		else
			return (WCHAR)(BYTE)ch[0];
	case Encoding::UTF_MBCS:
		{
			int nCh = m_encoding->GetUtf8TrailingCount(ch[0]) + 1;
			for (int i = 1; i < nCh; ++i)
				ch[i] = m_sqb->ReadByte();
			return m_encoding->DecodeUtf8Char(ch, nCh);
		}
		break;
	default:
		cdec_throw(IOException(EC_Unexpected));
	}
}

stringx StreamReader::ReadToEnd()
{
	std::wstring16 s;
	s.reserve(32);

	if (m_encoding->IsWideChar())
		ReadToEndWideChar(s);
	else
		ReadToEndMultiBytes(s);
	return stringx(s);
}

// TODO 需要优化
void StreamReader::ReadToEndWideChar(std::wstring16& s)
{
	while (!m_sqb->IsEnd())
		s.append(1, m_sqb->ReadInt16());
}

// TODO 需要优化
void StreamReader::ReadToEndMultiBytes(std::wstring16& s)
{
	while (!m_sqb->IsEnd())
		s.append(1, ReadCharMultiBytes());
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
