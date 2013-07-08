#pragma once

#ifndef __CDECCORE__
#define __CDECCORE__

// -------------------------------------------------------------------------- //
// Predef
// -------------------------------------------------------------------------- //

#include "cdecegg.h"

#ifdef CDECCORE_EXPORTS
#	define CDECCOREEXPORT		DECLSPEC_EXPORT
#else
#	define CDECCOREEXPORT		DECLSPEC_IMPORT
#endif

#define CDECCOREAPI(type)	EXTERN_C CDECCOREEXPORT type __stdcall

// -------------------------------------------------------------------------- //
// C-- Essential
// -------------------------------------------------------------------------- //

#include "cdeccore/essential/predef.h"
#include "cdeccore/essential/ref.h"
#include "cdeccore/essential/gc.h"
#include "cdeccore/essential/foreach.h"
#include "cdeccore/essential/interop.h"
#include "cdeccore/essential/string.h"
#include "cdeccore/collection/array.h"

// -------------------------------------------------------------------------- //
// C-- Library - Common
// -------------------------------------------------------------------------- //

#include "cdeccore/common/stringbuilder.h"
#include "cdeccore/common/datetime.h"
#include "cdeccore/common/console.h"
#include "cdeccore/common/math.h"
#include "cdeccore/common/converter.h"
#include "cdeccore/common/bitconverter.h"

// -------------------------------------------------------------------------- //
// C-- Library - Collections
// -------------------------------------------------------------------------- //

#include "cdeccore/collection/vector.h"
#include "cdeccore/collection/map.h"

// -------------------------------------------------------------------------- //
// C-- Library - Text
// -------------------------------------------------------------------------- //

#include "cdeccore/text/encoding.h"

// -------------------------------------------------------------------------- //
// C-- Library - Threading
// -------------------------------------------------------------------------- //

#include "cdeccore/threading/criticalsection.h"

// -------------------------------------------------------------------------- //
// C-- Library - IO
// -------------------------------------------------------------------------- //

#include "cdeccore/io/directory.h"
#include "cdeccore/io/stream.h"
#include "cdeccore/io/textreader.h"
#include "cdeccore/io/textwriter.h"
#include "cdeccore/io/mapping.h"

// -------------------------------------------------------------------------- //
// C-- Library - Crypto
// -------------------------------------------------------------------------- //

#include "cdeccore/crypto/hash.h"
#include "cdeccore/crypto/crypto.h"

// -------------------------------------------------------------------------- //
// C-- Library - XMLCE
// -------------------------------------------------------------------------- //

#include "cdeccore/xmlce/xmlreader.h"
#include "cdeccore/xmlce/xmlwriter.h"
#include "cdeccore/xmlce/xmldom.h"

// -------------------------------------------------------------------------- //
// C-- Library - Json
// -------------------------------------------------------------------------- //

#include "cdeccore/json/jsondom.h"

// -------------------------------------------------------------------------- //

#endif
