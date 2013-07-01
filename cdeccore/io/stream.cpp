#include "stdafx.h"
#include "filewrap.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

void FileStream::Open(stringx filename, AccessMode access, ShareMode share, bool fCreate)
{
	if (filename == NULL || filename.Length() == 0)
		cdec_throw(IOException(EC_InvalidArg));

	if (m_pFile != NULL && m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_AlreadyOpened));

	m_pFile = new FileWrapper();
	m_pFile->Open(filename.c_str(), (FileWrapper::AccessMode)access, (FileWrapper::ShareMode)share, fCreate);
}

void __stdcall FileStream::Close()
{
	if (m_pFile != NULL)
	{
		m_pFile->Close();
		m_pFile = NULL;
	}
}

UINT FileStream::Read(void* buffer, UINT cbToRead)
{
	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));
	return m_pFile->Read(buffer, cbToRead);
}

UINT FileStream::AtomRead(void* buffer, UINT cbToRead,UINT64 pos)
{
	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));
	return m_pFile->AtomRead(buffer, cbToRead,pos);
}


UINT FileStream::Read(ref<ArrayV<BYTE> > buffer, size_t cbToRead, size_t offset)
{
	if (offset + cbToRead > buffer->Count())
		cdec_throw(IOException(EC_OutOfRange));

	pin_ptr<BYTE> pinBuf = buffer->GetBuffer();
	return Read(pinBuf.ptr() + offset, cbToRead);
}

UINT FileStream::Write(const void* buffer, UINT cbToWrite)
{
	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));
	return m_pFile->Write(buffer, cbToWrite);
}

UINT FileStream::AtomWrite(const void* buffer, UINT cbToWrite,UINT64 pos)
{
	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));
	return m_pFile->AtomWrite(buffer, cbToWrite,pos);
}

UINT FileStream::Write(ref<ArrayV<BYTE> > buffer, size_t cbToWrite, size_t offset)
{
	if (offset + cbToWrite > buffer->Count())
		cdec_throw(IOException(EC_OutOfRange));

	pin_ptr<BYTE> pinBuf = buffer->GetBuffer();
	return Write(pinBuf.ptr() + offset, cbToWrite);
}

INT64 FileStream::Seek(INT64 pos, SeekPosition cp)
{
	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));
	return m_pFile->Seek(pos, (FileWrapper::SeekOrigin)cp);
}

INT64 FileStream::Length()
{
	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));
	return m_pFile->Size();
}

void FileStream::SetLength(INT64 length)
{
	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));
	m_pFile->SetLength(length);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
