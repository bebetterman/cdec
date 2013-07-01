#include "stdafx.h"
#include "md5.h"
#include "sha1.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

MD5::MD5() { m_impl = new kfcimpl::MD5(); }

MD5::~MD5() { delete m_impl; }

void MD5::Reset()
{
	m_impl->reset();
}

void MD5::Update(const void* input, int size)
{
	m_impl->update((BYTE*)input, size);
}

void MD5::Final(BYTE* digest)
{
	m_impl->final(digest);
}

// -------------------------------------------------------------------------- //

SHA1::SHA1() { m_impl = new kfcimpl::SHA1(); }

SHA1::~SHA1() { delete m_impl; }

void SHA1::Reset()
{
	m_impl->reset();
}

void SHA1::Update(const void* input, int size)
{
	m_impl->update((BYTE*)input, size);
}

void SHA1::Final(BYTE digest[20])
{
	m_impl->final(digest);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
