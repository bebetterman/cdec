// -------------------------------------------------------------------------- //
//	文件名		：	report_sax.cpp
//	创建者		：	杨钢
//	创建时间	：	2006-3-2 11:23:40
//	功能描述	：	报表服务的写 XML 实现
//
// -------------------------------------------------------------------------- //

#include "stdafx.h"
//#include "xmlwriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDEC_NS_BEGIN

#if 0
// -------------------------------------------------------------------------- //

WCHAR DbgTextWriterEx::buffer[4096];		// 绝对不可以重入

// -------------------------------------------------------------------------- //

// 临时的解决方案

void __KFC_EXPORT StrMbcsToWide(const diag_string& s, diag_wstring& t)
{
	int	m = s.size() + 1;
	std::vector<WCHAR> vecBuf(m);
	int n = MultiByteToWideChar(0, 0, s.c_str(), s.length(), &vecBuf[0], vecBuf.size());
	t = &vecBuf[0];
	ASSERT(n <= m);
}

void StrWideToMbcs(const diag_wstring& s, diag_string& t)
{
	int	m = s.size()*2 + 1;
	std::vector<CHAR> vecBuf(m);
	int n = wcstombs(&vecBuf[0], s.c_str(), m);
	t = &vecBuf[0];
	ASSERT(n <= m);
}

// -------------------------------------------------------------------------- //

DbgTextWriter::DbgTextWriter(stringx filename, ref<Encoding> encoding):
m_ref(1), m_encoding(encoding)
{
	m_stream = gc_new<FileStream>(filename, FileStream::AccessReadWrite, true);
	// BOF header is ommited now
}

void DbgTextWriter::Close()
{
	m_stream->Close();
}

void DbgTextWriter::Write(PCWSTR pwszValue)
{
	std::string buf = m_encoding->FromUnicode(pwszValue, wcslen(pwszValue));
	UINT cbWriten = m_stream->Write(buf.data(), buf.size());
	ASSERT(cbWriten == buf.size());
}

#endif
// -------------------------------------------------------------------------- //
CDEC_NS_END
