#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

ServerResponse::ServerResponse(UINT state, const void* data, int count, bool fConstance)
{
	m_state = state;
	MHD_ResponseMemoryMode mm = fConstance ? MHD_RESPMEM_PERSISTENT : MHD_RESPMEM_MUST_COPY;
	m_inner = MHD_create_response_from_buffer(count, (void*)data, mm);
}

void ServerResponse::Close()
{
	m_state = 0;
	if (m_inner != NULL)
	{
		MHD_destroy_response(m_inner);
		m_inner = NULL;
	}
}

void ServerResponse::AddHeader(PCSTR key, PCSTR value)
{
	if (MHD_add_response_header(m_inner, key, value) != MHD_YES)
		cdec_throw(Exception(EC_Fail));
}

void ServerResponse::AddHeader(PCSTR key, stringx value)
{
	ref<Encoding> e = Encoding::get_UTF8();
	std::string valuea = e->FromUnicode(value);
	AddHeader(key, valuea.c_str());
}

void ServerResponse::AddHeader(stringx key, stringx value)
{
	ref<Encoding> e = Encoding::get_UTF8();
	std::string keya = e->FromUnicode(key);
	std::string valuea = e->FromUnicode(value);
	AddHeader(keya.c_str(), valuea.c_str());
}

void ServerResponse::RemoveHeader(PCSTR key, PCSTR value)
{
	if (MHD_del_response_header(m_inner, key, value) != MHD_YES)
		cdec_throw(Exception(EC_Fail));
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
