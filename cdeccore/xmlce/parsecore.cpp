#include "stdafx.h"
#include "parsecore.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

ParserMicroCore::ParserMicroCore(): m_pAcpt(NULL), m_lxs(LXS_Invalid)
{
}

ParserMicroCore::~ParserMicroCore()
{
}

void ParserMicroCore::Reset(ref<ISaxAccepter> pAcpt)
{
	ASSERT(m_pAcpt == NULL && pAcpt != NULL);
	m_pAcpt = pAcpt;
	m_lxs = LXS_LT;

	pAcpt->BeginDocument();
}

void ParserMicroCore::SendChar(WCHAR ch)
{
	switch (m_lxs)
	{
	case LXS_LT:
		if (ch == '<')
			m_lxs = LXS_LTA;
		else if (IsSpace(ch))
			;
		else if (ch == '&')		// Escape char
			BeginEsq();
		else if (IsValueChar(ch))
		{
			m_Identifier = ch;
			m_lxs = LXS_VT;
		}
		else
			throw LXS_LT;
		break;

	case LXS_LTA:
		if (ch == '?')
			m_lxs = LXS_HT_DORL;
		else if (IsIdfHdr(ch))
		{
			m_Identifier = ch;
			m_lxs = LXS_EB_IDT;
		}
		else if (ch == '/')
			m_lxs = LXS_EE_IDT;
		else if (!IsSpace(ch))
			throw LXS_LTA;
		break;

	case LXS_HT_DORL:
		if (ch == '?')
			m_lxs = LXS_HT_GT;
		break;

	case LXS_HT_GT:
		if (ch == '>')
			m_lxs = LXS_LT;
		else if (!IsSpace(ch))
			throw LXS_HT_GT;
		break;

	case LXS_EB_IDT:
		if (IsIdf(ch))
		{
			m_Identifier += ch;
		}
		else if (IsSpace(ch))
		{
			m_lastElement = m_Identifier;
			m_pAcpt->BeginElement((stringx)m_Identifier);
			m_lxs = LXS_EB_IDTE;
		}
		else if (ch == '/')
		{
			m_pAcpt->BeginElement((stringx)m_Identifier);
			m_pAcpt->EndElement((stringx)m_Identifier);
			m_lxs = LXS_EB_GT;
		}
		else if (ch == '>')
		{
			m_pAcpt->BeginElement((stringx)m_Identifier);
			m_lxs = LXS_LT;
		}
		else
			throw LXS_EB_IDT;
		break;

	case LXS_EB_GT:
		if (ch == '>')
			m_lxs = LXS_LT;
		else if (!IsSpace(ch))
			throw LXS_EB_GT;
		break;

	case LXS_EB_IDTE:
		if (IsIdfHdr(ch))
		{
			m_Identifier = ch;
			m_lxs = LXS_AN_IDT;
		}
		else if (ch == '/')
		{
			m_pAcpt->EndElement(m_lastElement.c_str());
			m_lxs = LXS_EB_GT;
		}
		else if (ch == '>')
			m_lxs = LXS_LT;
		else if (!IsSpace(ch))
			throw LXS_EB_IDTE;
		break;

	case LXS_EE_IDT:
		if (IsIdfHdr(ch))
		{
			m_Identifier = ch;
			m_lxs = LXS_EE_IDTC;
		}
		else if (!IsSpace(ch))
			throw LXS_EE_IDT;
		break;

	case LXS_EE_IDTC:
		if (IsIdf(ch))
			m_Identifier += ch;
		else if (IsSpace(ch))
		{
			m_pAcpt->EndElement(m_Identifier.c_str());
			m_lxs = LXS_EE_IDTE;
		}
		else if (ch == '>')
		{
			m_pAcpt->EndElement(m_Identifier.c_str());
			m_lxs = LXS_LT;
		}
		else
			throw LXS_EE_IDTC;
		break;

	case LXS_EE_IDTE:
		if (ch == '>')
			m_lxs = LXS_LT;
		else
			throw LXS_EE_IDTE;
		break;

	case LXS_AN_IDT:
		if (IsIdf(ch))
			m_Identifier += ch;
		else if (IsSpace(ch))
		{
			m_attrName = m_Identifier;
			m_Identifier.resize(0, 0);	// m_Identifier.clear();
			m_lxs = LXS_AN_IDTE;
		}
		else if (ch == '=')
		{
			m_attrName = m_Identifier;
			m_Identifier.resize(0, 0);	// m_Identifier.clear();
			m_lxs = LXS_AV_BQ;
		}
		else
			throw LXS_AN_IDT;
		break;

	case LXS_AN_IDTE:
		if (ch == '=')
			m_lxs = LXS_AV_BQ;
		else if (!IsSpace(ch))
			throw LXS_AN_IDTE;
		break;

	case LXS_AV_BQ:
		if (ch == '\"')
			m_lxs = LXS_AV_VT;
		else if (!IsSpace(ch))
			throw LXS_AV_BQ;
		break;

	case LXS_AV_VT:
		if (ch == '\"')
		{
			m_pAcpt->AddAttribute(m_attrName.c_str(), m_Identifier.c_str());
			m_lxs = LXS_EB_IDTE;
		}
		else if (ch == '&')	// Escape char
			BeginEsq();
		else
			m_Identifier += ch;
		break;

	case LXS_VT:
		if (ch == '<')
		{
			m_pAcpt->AddValue(m_Identifier.c_str());
			m_lxs = LXS_LTA;
		}
		else if (ch == '&')	// Escape char
			BeginEsq();
		else if (IsValueChar(ch))
			m_Identifier += ch;
		else
			throw LXS_VT;
		break;

	case LXS_ESQ:
		if (ch != ';')
			m_Identifier += ch;
		else
			EndEsq();
		break;
	}
}

void ParserMicroCore::Finalize()
{
	if (m_lxs != LXS_LT)
		throw LXS_LT;

	m_pAcpt->EndDocument();
}

// -------------------------------------------------------------------------- //

void ParserMicroCore::BeginEsq()
{
	m_esqSaveState = m_lxs;
	m_esqSaveId = m_Identifier;

	m_Identifier.resize(0, 0);	// m_Identifier.clear();
	m_lxs = LXS_ESQ;
}

void ParserMicroCore::EndEsq()
{
	WCHAR	chEsq = 0;
	if (m_Identifier == __X("amp"))
		chEsq = __X('&');
	else if (m_Identifier == __X("lt"))
		chEsq = __X('<');
	else if (m_Identifier == __X("gt"))
		chEsq = __X('>');
	else if (m_Identifier == __X("quot"))
		chEsq = __X('\"');
	else if (m_Identifier[0] == __X('#'))
	{
		PCWSTR p = NULL;
		if (m_Identifier[1] == __X('x'))
		{
			p = m_Identifier.c_str() + 2;
			chEsq = (WCHAR)ToInt32(p, 16);
		}
		else 
		{
			p = m_Identifier.c_str() + 1;
			chEsq = (WCHAR)ToInt32(p, 10);
		}
	}
	else
		throw LXS_ESQ;

	m_Identifier = m_esqSaveId;
	m_Identifier += chEsq;
	m_lxs = m_esqSaveState;
}

// -------------------------------------------------------------------------- //

int ParserMicroCore::ToInt32(PCWSTR text, int base)
{
	if (base < 0 || base > 36)
		cdec_throw(Exception(EC_OutOfRange));

	int value = 0;
	while (*text != 0)
	{
		int digit = ParseDigit(*text++);
		if (digit == -1)
			break;

		if (digit >= base)
			cdec_throw(Exception(EC_OutOfRange));

		// TODO Overflow checking
		value = value * base + digit;
	}

	return value;
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
