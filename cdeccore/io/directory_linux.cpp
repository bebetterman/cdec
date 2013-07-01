#include "stdafx.h"

#ifdef X_OS_LINUX

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

CDEC_NS_BEGIN
//---------------------------------------------------------------------------//

bool PxStat(stringx path, struct stat &fstat)
{
	std::string patha = Encoding::get_UTF8()->FromUnicode(path);
	if (stat(patha.c_str(), &fstat) == 0)
		return true;
	if (errno == ENOENT)
		return false;
	cdec_throw_stdc_lasterr(IOException);
}

void PxMove(stringx source, stringx target)
{
	std::string source_a = Encoding::get_UTF8()->FromUnicode(source);
	std::string target_a = Encoding::get_UTF8()->FromUnicode(target);

	struct stat fstat;
	if (stat(target_a.c_str(), &fstat) == 0)
		cdec_throw(IOException(EC_IO_AlreadyExists));
	if (rename(source_a.c_str(), target_a.c_str()) != 0)
		cdec_throw_stdc_lasterr(IOException);
}

bool File::Exists(stringx path)
{
	struct stat fstat;
	if (!PxStat(path, fstat))
		return false;

	mode_t stmode = fstat.st_mode;
	return S_ISREG(stmode);
}

bool File::Delete(stringx path)
{
	std::string patha = Encoding::get_UTF8()->FromUnicode(path);
	if (unlink(patha.c_str()) == 0 || errno == ENOENT)
		return true;
	cdec_throw_stdc_lasterr(IOException);
}

void File::Move(stringx pathOld, stringx pathNew)
{
	PxMove(pathOld, pathNew);
}

//---------------------------------------------------------------------------//

bool Directory::Exists(stringx path)
{
        struct stat fstat;
        if (!PxStat(path, fstat))
                return false;

        mode_t stmode = fstat.st_mode;
        return S_ISDIR(stmode);
}

void Directory::Create(stringx path)
{
	std::string patha = Encoding::get_UTF8()->FromUnicode(path);
	const mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP;

	if (0 != mkdir(patha.c_str(), mode) && errno != EEXIST)
		cdec_throw_stdc_lasterr(IOException);
}

void Directory::Delete(stringx path)
{
        std::string patha = Encoding::get_UTF8()->FromUnicode(path);
	if (0 != rmdir(patha.c_str()))
                cdec_throw_stdc_lasterr(IOException);
}

void Directory::Move(stringx pathOld, stringx pathNew)
{
        PxMove(pathOld, pathNew);
}

ref<StringArrayList> Directory::GetDirectories(stringx path)
{
        ref<StringArrayList> r = gc_new<StringArrayList>();
        ref<Encoding> e = Encoding::get_UTF8();
        std::string patha = e->FromUnicode(path);

        DIR *dir;
        struct dirent *dirp;

        if((dir = opendir(patha.c_str())) == NULL)
                cdec_throw_stdc_lasterr(IOException);

        while((dirp = readdir(dir)) != NULL)
        {
		if (dirp->d_type != DT_DIR)
			continue;

                const char* name_a = dirp->d_name;
                if (name_a[0] == '.' && (name_a[1] == '.' || name_a[1] == 0))
                        continue;

                stringx name = e->ToUnicode(dirp->d_name);
                r->Add(name);
        }

        closedir(dir);
        return r;
}

ref<StringArrayList> Directory::GetFiles(stringx path)
{
        ref<StringArrayList> r = gc_new<StringArrayList>();
        ref<Encoding> e = Encoding::get_UTF8();
        std::string patha = e->FromUnicode(path);

        DIR *dir;
        struct dirent *dirp;

        if((dir = opendir(patha.c_str())) == NULL)
                cdec_throw_stdc_lasterr(IOException);

        while((dirp = readdir(dir)) != NULL)
        {
                if (dirp->d_type != DT_REG)
                        continue;

                const char* name_a = dirp->d_name;
                if (name_a[0] == '.' && (name_a[1] == '.' || name_a[1] == 0))
                        continue;

                stringx name = e->ToUnicode(dirp->d_name);
                r->Add(name);
        }

        closedir(dir);
        return r;
}

ref<StringArrayList> Directory::GetFileSystemEntries(stringx path)
{
	ref<StringArrayList> r = gc_new<StringArrayList>();
	ref<Encoding> e = Encoding::get_UTF8();
	std::string patha = e->FromUnicode(path);

	DIR *dir;
        struct dirent *dirp;

        if((dir = opendir(patha.c_str())) == NULL)
		cdec_throw_stdc_lasterr(IOException);

        while((dirp = readdir(dir)) != NULL)
	{
		const char* name_a = dirp->d_name;
		if (name_a[0] == '.' && (name_a[1] == '.' || name_a[1] == 0))
			continue;

		stringx name = e->ToUnicode(dirp->d_name);
		r->Add(name);
	}

        closedir(dir);
	return r;
}

stringx Directory::GetCurrentDir()
{
	char buff[PATH_MAX];
	if (getcwd(buff, PATH_MAX) == NULL)
		cdec_throw_stdc_lasterr(IOException);
	return Encoding::get_UTF8()->ToUnicode(buff);
}

//---------------------------------------------------------------------------//
CDEC_NS_END

#endif
