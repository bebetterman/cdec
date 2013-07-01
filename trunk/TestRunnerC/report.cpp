// -------------------------------------------------------------------------- //
//	�ļ���		��	report.cpp
//	������		��	���
//	����ʱ��	��	4/22/2008 9:24:02 AM
//	��������	��
//
// -------------------------------------------------------------------------- //

#include "stdafx.h"
#include "report.h"

// -------------------------------------------------------------------------- //

void ReportBase::EndCase(int r)
{
	++_nCases;
	_nError += r != ResultOK ? 1 : 0;
	_nameCase = NULL;
}

// -------------------------------------------------------------------------- //

ReportConsole::ReportConsole()
{
	// MSDN û����ȷ˵�����������ӣ���� HANDLE ���ͷ�
#ifdef X_OS_WINDOWS
	_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(_hStdout, &csbiInfo);
	_attrNormal = csbiInfo.wAttributes;
#else // linux
	//
#endif
}

void ReportConsole::EndSession()
{
	puts("\r\n");

#ifdef X_OS_WINDOWS
	SetConsoleTextAttribute(_hStdout, _attrNormal);
#else // linux
	//
#endif

	printf("Succeeded\t%d\nFailed\t\t%d\n", _nCases - _nError, _nError);

	base::EndSession();
}

void ReportConsole::EndCase(int r)
{
	char	ch = r == ResultOK ? '.' : 'X';

#ifdef X_OS_WINDOWS
	DWORD	attr = (r == ResultOK ? FOREGROUND_GREEN : FOREGROUND_RED) | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(_hStdout, attr);
	_putch(ch);	// VC6 �� Console ����û�� Unicode �汾
#else // linux
	putchar(ch);
#endif

	base::EndCase(r);
}

// -------------------------------------------------------------------------- //

void ReportConsoleVerbose::BeginSession() { puts("Test begins."); }

void ReportConsoleVerbose::EndSession() { puts("Test done."); }

void ReportConsoleVerbose::BeginSuite(const char* nameSuite) { m_nameSuite = nameSuite; }

void ReportConsoleVerbose::BeginCase(const char* nameCase) { printf((m_nameSuite + nameCase).c_str()); }

void ReportConsoleVerbose::EndCase(int r) { puts(r == ResultOK ? " ... OK" : " ... Fail"); }

// -------------------------------------------------------------------------- //

void ReportXml::BeginSession()
{
	base::BeginSession();

	ASSERT(_fp == NULL);
	_fp = fopen(_filename, "w");

	ASSERT(_fp != NULL);
	fputc(0xFEFF, _fp);

	fputs("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n", _fp);
	fputs("<session>\r\n", _fp);
}

void ReportXml::EndSession()
{
	ASSERT(_fp != NULL);
	fputs("</session>\r\n", _fp);
	fclose(_fp);
	_fp = NULL;

	base::EndSession();
}

void ReportXml::BeginSuite(const char* nameSuite)
{
	base::BeginSuite(nameSuite);
	fprintf(_fp, "<suite name=\"%s\">\r\n", nameSuite);
}

void ReportXml::EndSuite()
{
	fputs("</suite>\r\n", _fp);
	base::EndSuite();
}

void ReportXml::BeginCase(const char* nameCase)
{
	base::BeginCase(nameCase);
}

void ReportXml::EndCase(int r)
{
	fprintf(_fp, "<case name=\"%s\" succ=\"%d\" />\r\n",
		_nameCase, r == ResultOK ? 1 : 0);
	base::EndCase(r);
}

// -------------------------------------------------------------------------- //
