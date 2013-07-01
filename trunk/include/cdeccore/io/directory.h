#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT Path: public Object
{
	DECLARE_REF_CLASS(Path)

public:
	static stringx	Combine(stringx pathBase, stringx pathRel);
	static stringx	GetFileName(stringx path);
};

// -------------------------------------------------------------------------- //

class CDECCOREEXPORT File : public Object
{
	DECLARE_REF_CLASS(File)

public:
	static bool		Exists(stringx path);
	static bool		Delete(stringx path);
	static void		Move(stringx pathOld, stringx pathNew);
};

// -------------------------------------------------------------------------- //

class CDECCOREEXPORT Directory : public Object
{
	DECLARE_REF_CLASS(Directory)

public:
	static bool		Exists(stringx path);
	static void		Create(stringx path);
	static void		Delete(stringx path);
	static void		Move(stringx pathOld, stringx pathNew);

	static ref<StringArrayList> GetDirectories(stringx path);
	static ref<StringArrayList> GetFiles(stringx path);
	static ref<StringArrayList> GetFileSystemEntries(stringx path);

	static stringx	GetCurrentDir();

};

//---------------------------------------------------------------------------//
CDEC_NS_END
