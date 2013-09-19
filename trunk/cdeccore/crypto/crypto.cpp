#include "stdafx.h"
#include "md5.h"
#include "sha1.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

ref<ByteArray> HashAlgorithmCompute(interface HashAlgorithm* pThis, ref<Stream> istream)
{
	const int BufferSize = 0x100000;
	void* buffer = CoreAllocLC(BufferSize);
	istream->Seek(0, Stream::SeekBegin);
	pThis->Reset();
	int cbop;
	while ((cbop = istream->Read(buffer, BufferSize)) != 0)
		pThis->Update(buffer, cbop);
	CoreFreeLC(buffer, BufferSize);
	return pThis->Final();
}

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
