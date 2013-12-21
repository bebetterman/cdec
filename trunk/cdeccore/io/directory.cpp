#include "stdafx.h"

CDEC_NS_BEGIN

//---------------------------------------------------------------------------//

#ifdef X_OS_WINDOWS
#	define PATH_SEPERATOR_CHAR	'\\'
#else
#	define PATH_SEPERATOR_CHAR	'/'
#endif

stringx Path::Combine(stringx pathBase, stringx pathRel)
{
	// TODO To be implemented
	return pathBase + PATH_SEPERATOR_CHAR + pathRel;
}

stringx Path::GetFileName(stringx path)
{
	static WCHAR seps[] = { '\\', '/' };
	int pos = path.LastIndexOfAny(seps, 2);
	return pos >= 0 ? path.Substring(pos + 1) : path;
}

//---------------------------------------------------------------------------//

#ifdef X_OS_WINDOWS
//---------------------------------------------------------------------------//

DWORD ApiGetFileAttr(stringx path)
{
	DWORD dwState = GetFileAttributesW(path.c_str());
	if (dwState == INVALID_FILE_ATTRIBUTES)
	{
		DWORD err = GetLastError();
		if (err != ERROR_FILE_NOT_FOUND && err != ERROR_PATH_NOT_FOUND)
			cdec_throw_win32_err(IOException, err);
	}
	return dwState;
}

//---------------------------------------------------------------------------//

bool File::Exists(stringx path)
{
	DWORD dwState = ApiGetFileAttr(path);
	return dwState != INVALID_FILE_ATTRIBUTES && !(dwState & FILE_ATTRIBUTE_DIRECTORY);
}

bool File::Delete(stringx path)
{
	if (::DeleteFile(path.c_str()) != 0)
		return true;

	DWORD	err = GetLastError();
	if (err == ERROR_FILE_NOT_FOUND)
		return false;

	cdec_throw_win32_err(IOException, err);

}

void File::Move(stringx pathOld, stringx pathNew)
{
	if (!::MoveFile(pathOld.c_str(), pathNew.c_str()))
		cdec_throw_win32_lasterr(IOException);
}

//---------------------------------------------------------------------------//

bool Directory::Exists(stringx path)
{
	DWORD dwState = ApiGetFileAttr(path);
	return dwState != INVALID_FILE_ATTRIBUTES && (dwState & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

void Directory::Create(stringx path)
{
	if (::CreateDirectory(path.c_str(), NULL) == 0)
	{
		DWORD err = GetLastError();
		if (err != ERROR_ALREADY_EXISTS)
			cdec_throw_win32_err(IOException, err);
	}
}

void Directory::Delete(stringx path)
{
	if (::RemoveDirectory(path.c_str()) == 0)
		cdec_throw_win32_lasterr(IOException);
}

void Directory::Move(stringx pathOld, stringx pathNew)
{
	if (::MoveFile(pathOld.c_str(), pathNew.c_str()) == 0)
		cdec_throw_win32_lasterr(IOException);
}

ref<StringArrayList> Directory::GetDirectories(stringx path)
{
	ref<StringArrayList> Directories = gc_new<StringArrayList>();

	WIN32_FIND_DATAW info;
	memset(&info,0,sizeof(info));

	stringx pathname = path + __X("\\*");
	HANDLE handle = FindFirstFile(pathname.c_str(), &info);
	if (handle == NULL)
		cdec_throw_win32_lasterr(IOException);

	do
	{
		if (cdec_egg::IsBitUsed(info.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY))
		{
			stringx name = info.cFileName;
			if ( name != __X(".") && name != __X("..") )
				Directories->Add(name);
		}
	} while (FindNextFile(handle, &info));

	FindClose(handle);
	return Directories;
}

ref<StringArrayList> Directory::GetFiles(stringx path)
{
	ref<StringArrayList> files = gc_new<StringArrayList>();

	WIN32_FIND_DATAW info;
	memset(&info,0,sizeof(info));

	stringx pathname = path + __X("\\*");
	HANDLE handle = FindFirstFile(pathname.c_str(), &info);
	if (handle == NULL)
		cdec_throw_win32_lasterr(IOException);

	do
	{
		if (!cdec_egg::IsBitUsed(info.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_DEVICE))
		{
			stringx name = info.cFileName;
			files->Add(name);
		}
	} while (FindNextFile(handle, &info));

	FindClose(handle);
	return files;
}

ref<StringArrayList> Directory::GetFileSystemEntries(stringx path)
{
	ref<StringArrayList> entries = gc_new<StringArrayList>();

	WIN32_FIND_DATAW info;
	memset(&info,0,sizeof(info));

	stringx pathname = path + __X("\\*");
	HANDLE handle = FindFirstFile(pathname.c_str(), &info);
	if (handle == NULL)
		cdec_throw_win32_lasterr(IOException);

	do
	{
		if (cdec_egg::IsBitUsed(info.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY))
		{
			stringx name = info.cFileName;
			if ( name != __X(".") && name != __X("..") )
				entries->Add(name);
		}
		else if (!cdec_egg::IsBitUsed(info.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_DEVICE))
		{
			stringx name = info.cFileName;
			entries->Add(name);
		}
		else
		{
			continue;
		}
	} while (FindNextFile(handle, &info));

	FindClose(handle);
	return entries;
}

stringx Directory::GetCurrentDir()
{
	WCHAR* buf = (WCHAR*)CoreAllocT(MAX_PATH);
	DWORD dwRet = ::GetCurrentDirectory(MAX_PATH, buf);

	if (dwRet == 0)
	{
		CoreFreeT(buf, MAX_PATH);
		cdec_throw_win32_lasterr(IOException);
	}

	stringx curpath = buf;
	CoreFreeT(buf, MAX_PATH);
	return curpath;
}

//---------------------------------------------------------------------------//
#else

//---------------------------------------------------------------------------//
#endif

CDEC_NS_END
