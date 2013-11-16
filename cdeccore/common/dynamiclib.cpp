#include "stdafx.h"

#ifndef X_OS_WINDOWS
#include "dlfcn.h"
#endif

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS

bool DynamicLibrary::Load(stringx moduleName)
{
	m_hModule = LoadLibraryW(moduleName.c_str());
	return m_hModule != NULL;
}

void* DynamicLibrary::GetAddress(stringx exportName)
{
	ref<Encoding> utf8 = Encoding::get_UTF8();
	std::string exportNameS = utf8->FromUnicode(exportName);
	return ::GetProcAddress((HMODULE)m_hModule, exportNameS.c_str());
}

#else

bool DynamicLibrary::Load(stringx moduleName)
{
	ref<Encoding> utf8 = Encoding::get_UTF8();
	std::string moduleNameS = utf8->FromUnicode(moduleName);
	m_hModule = dlopen(moduleNameS.c_str(), RTLD_LAZY | RTLD_GLOBAL);
	return m_hModule != NULL;
}

void* DynamicLibrary::GetAddress(stringx exportName)
{
	ref<Encoding> utf8 = Encoding::get_UTF8();
	std::string exportNameS = utf8->FromUnicode(exportName);
	return ::dlsym(m_hModule, exportNameS.c_str());
}

#endif

// -------------------------------------------------------------------------- //
CDEC_NS_END
