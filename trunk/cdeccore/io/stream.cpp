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

void FileStream::Flush()
{
	m_pFile->Flush();
}

void FileStream::Close()
{
	if (m_pFile != NULL)
	{
		m_pFile->Close();
		m_pFile = NULL;
	}
}

int FileStream::Read(void* buffer, int count)
{
	if (count == 0)
		return 0;

	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));

	return m_pFile->Read(buffer, count);
}

int FileStream::AtomRead(INT64 pos, void* buffer, int count)
{
	if (count == 0)
		return 0;

	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));

	return m_pFile->AtomRead(pos, buffer, count);
}

int FileStream::Read2(ref<ByteArray> buffer, int offset, int count)
{
	if (count == 0)
		return 0;

	if (CheckOutOfRange(offset, count, buffer->Count()))
		cdec_throw(IOException(EC_OutOfRange));

	pin_ptr<BYTE> pinBuffer = buffer->GetBuffer();
	return Read(pinBuffer.ptr() + offset, count);
}

int FileStream::Write(const void* buffer, int count)
{
	if (count == 0)
		return 0;

	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));

	return m_pFile->Write(buffer, count);
}

int FileStream::AtomWrite(INT64 pos, const void* buffer, int count)
{
	if (count == 0)
		return 0;

	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));

	return m_pFile->AtomWrite(pos, buffer, count);
}

int FileStream::Write2(ref<ByteArray> buffer, int offset, int count)
{
	if (count == 0)
		return 0;

	if (CheckOutOfRange(offset, count, buffer->Count()))
		cdec_throw(IOException(EC_OutOfRange));

	pin_ptr<BYTE> pinBuffer = buffer->GetBuffer();
	return Write(pinBuffer.ptr() + offset, count);
}

INT64 FileStream::Seek(INT64 pos, SeekPosition cp)
{
	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));
	return m_pFile->Seek(pos, (FileWrapper::SeekOrigin)cp);
}

INT64 FileStream::Position()
{
	if (!m_pFile->IsOpen())
		cdec_throw(IOException(EC_IO_NotOpened));
	return m_pFile->Pos();
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
