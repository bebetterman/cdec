#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

enum LexicalState
{
	LXS_Invalid,		// û��׼���ý����ַ�
	LXS_LT,				// ׼�����ձ�ʾһ����ʼ�� <
	LXS_LTA,			// ׼�������� < ֮������ݣ��������� Xml ͷ����ʼ��ر�Ԫ�أ���ע��

	LXS_HT_DORL,		// �ڴ� Xml ͷ�رձ�ǩ ?> �� ?
	LXS_HT_GT,			// �ڴ� Xml ͷ�رձ�ǩ ?> �� >

	LXS_EB_IDT,			// ����Ԫ�ص�����
	LXS_EB_IDTE,		// ����֮��Ŀո�
	LXS_EB_GT,			// ��ʼԪ�ؽ����� >

	LXS_EE_IDT,			// ����Ԫ�عرյ����ƿ�ʼ
	LXS_EE_IDTC,		// ����Ԫ�عرյ�����
	LXS_EE_IDTE,		// �ر�Ԫ�ؽ����Ŀո�

	LXS_AN_IDT,			// �������Ե�����
	LXS_AN_IDTE,		// �������ƴ�����ϣ��ȴ� = ��
	LXS_AV_BQ,			// �ȴ���ʾֵ�Ŀ�ʼ�ĵȺ�
	LXS_AV_VT,			// ��������ֵ

	LXS_VT,				// ������Ϊֵ���ַ���

	LXS_ESQ,			// ת�������
};

// ������
// �벻Ҫֱ��ʹ�ô��࣬���������ṩ�ĺ������߲��� XmlDom ģ��
class ParserMicroCore
{
private:
	ref<ISaxAccepter>	m_pAcpt;
	LexicalState		m_lxs;

	std::wstring16		m_Identifier;

	LexicalState		m_esqSaveState;
	std::wstring16		m_esqSaveId;

	// Ϊ�˽��д��һ�������Ժ�ֱ�ӹرձ�ǩ������
	// ֻ�����ڲ��ǩ���ܱ�ֱ�ӹر�
	std::wstring16		m_lastElement;

	// Ϊ���������������ֵͬʱ���������󣬼�¼������
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
	static bool IsSpace(WCHAR ch)	// �Ƿ�Ϊ�ո�
	{
		return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
	}
	static bool IsIdfHdr(WCHAR ch)	// �Ƿ�Ϊ��ʶ����ͷ
	{
		return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_';
	}
	static bool IsIdf(WCHAR ch)		// �Ƿ�Ϊ��ʶ��
	{
		return IsIdfHdr(ch) || (ch >= '0' && ch <= '9') || ch == '-' || ch == ':';
	}
	static bool IsValueChar(WCHAR ch)	// ������Ϊֵ���ַ�
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
