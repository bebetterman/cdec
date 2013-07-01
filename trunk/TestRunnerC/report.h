// -------------------------------------------------------------------------- //
//	文件名		：	report.h
//	创建者		：	杨钢
//	创建时间	：	4/22/2008 9:23:59 AM
//	功能描述	：
//
// -------------------------------------------------------------------------- //

#pragma once

// -------------------------------------------------------------------------- //

interface __novtable IReport
{
	virtual void	BeginSession() = 0;
	virtual void	EndSession() = 0;

	virtual void	BeginSuite(const char* nameSuite) = 0;
	virtual void	EndSuite() = 0;

	virtual void	BeginCase(const char* nameCase)  = 0;
	virtual void	EndCase(int r) = 0;

	virtual void	AssertReport(const char* message, const char* file, int line) = 0;

	virtual ~IReport() {}
};

class __novtable ReportBase: public IReport
{
protected:
	const char*	_nameSuite, *_nameCase;
	int			_nCases, _nError;

public:
	ReportBase(): _nameSuite(NULL), _nameCase(NULL), _nCases(0), _nError(0) {}
	~ReportBase() {}

	void	BeginSession() { _nCases = _nError = 0; }
	void	EndSession() {}

	void	BeginSuite(const char* nameSuite) { _nameSuite = nameSuite; }
	void	EndSuite() { _nameSuite = NULL; }

	void	BeginCase(const char* nameCase) { _nameCase = nameCase; }
	void	EndCase(int r);

	void	AssertReport(const char* message, const char* file, int line) {}
};

class ReportConsole: public ReportBase
{
	typedef ReportBase base;

#ifdef X_OS_WINDOWS
	HANDLE	_hStdout;
	WORD	_attrNormal;
#endif

public:
	ReportConsole();
	void	EndSession();
	void	EndCase(int r);
};

class ReportConsoleVerbose: public ReportBase
{
	std::string	m_nameSuite;

public:
	void	BeginSession();
	void	EndSession();

	void	BeginSuite(const char* nameSuite);
	void	EndSuite() { m_nameSuite.clear(); }

	void	BeginCase(const char* nameCase);
	void	EndCase(int r);
};

class ReportXml: public ReportBase
{
	typedef ReportBase base;

	const char*	_filename;
	FILE*	_fp;

public:
	ReportXml(const char* filename): _filename(filename), _fp(NULL) {}
	~ReportXml() { ASSERT(_fp == NULL); }

	void	BeginSession();
	void	EndSession();

	void	BeginSuite(const char* nameSuite);
	void	EndSuite();

	void	BeginCase(const char* nameCase);
	void	EndCase(int r);
};

// -------------------------------------------------------------------------- //
