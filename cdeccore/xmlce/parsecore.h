#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

enum LexicalState
{
	LXS_Invalid,		// 没有准备好接收字符
	LXS_LT,				// 准备接收表示一个开始的 <
	LXS_LTA,			// 准备接收在 < 之后的内容，其后可能是 Xml 头，开始或关闭元素，或注释

	LXS_HT_DORL,		// 期待 Xml 头关闭标签 ?> 的 ?
	LXS_HT_GT,			// 期待 Xml 头关闭标签 ?> 的 >

	LXS_EB_IDT,			// 处理元素的名称
	LXS_EB_IDTE,		// 名称之后的空格
	LXS_EB_GT,			// 开始元素结束的 >

	LXS_EE_IDT,			// 处理元素关闭的名称开始
	LXS_EE_IDTC,		// 处理元素关闭的名称
	LXS_EE_IDTE,		// 关闭元素结束的空格

	LXS_AN_IDT,			// 接收属性的名称
	LXS_AN_IDTE,		// 属性名称处理完毕，等待 = 号
	LXS_AV_BQ,			// 等待表示值的开始的等号
	LXS_AV_VT,			// 接收属性值

	LXS_VT,				// 接收作为值的字符串

	LXS_ESQ,			// 转义符内容
};

// 解析器
// 请不要直接使用此类，改用下面提供的函数或者采用 XmlDom 模型
class ParserMicroCore
{
private:
	ref<ISaxAccepter>	m_pAcpt;
	LexicalState		m_lxs;

	std::wstring16		m_Identifier;

	LexicalState		m_esqSaveState;
	std::wstring16		m_esqSaveId;

	// 为了解决写完一长串属性后直接关闭标签的问题
	// 只有最内层标签才能被直接关闭
	std::wstring16		m_lastElement;

	// 为解决属性名与属性值同时给出的需求，记录属性名
	std::wstring16		m_attrName;

public:
	ParserMicroCore();
	~ParserMicroCore();

	void Reset(ref<ISaxAccepter>);
	void SendChar(WCHAR);
	void Finalize();

private:
	void BeginEsq();
	void EndEsq();

public:
	static bool IsSpace(WCHAR ch)	// 是否为空格
	{
		return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
	}
	static bool IsIdfHdr(WCHAR ch)	// 是否为标识符开头
	{
		return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_';
	}
	static bool IsIdf(WCHAR ch)		// 是否为标识符
	{
		return IsIdfHdr(ch) || (ch >= '0' && ch <= '9') || ch == '-' || ch == ':';
	}
	static bool IsValueChar(WCHAR ch)	// 可以作为值的字符
	{
		return ch != '>' && ch != '<' && ch != '&';
	}

public:
	static int ParseDigit(WCHAR ch)
	{
		if (ch >= '0' && ch <= '9')
			return ch - '0';
		else if (ch >= 'A' && ch <= 'Z')
			return ch - 'A' + 10;
		else if (ch >= 'a' && ch <= 'z')
			return ch - 'a' + 10;
		else
			return -1;
	}

	static int ToInt32(PCWSTR text, int base);
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
