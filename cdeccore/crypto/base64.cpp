#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

const char BASE64_CHAR_MAP[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

stringx Base64Encode(const BYTE* p, int len)
{
	std::wstring16 s;
	while (len >= 3)
	{
		// aaaaaabb bbbbcccc ccdddddd
		BYTE c1 = p[0], c2 = p[1], c3 = p[2];
		s.push_back(BASE64_CHAR_MAP[c1 >> 2]);
		s.push_back(BASE64_CHAR_MAP[((c1 & 3) << 4) | (c2 >> 4)]);
		s.push_back(BASE64_CHAR_MAP[((c2 & 15) << 2) | (c3 >> 6)]);
		s.push_back(BASE64_CHAR_MAP[c3 & 0x3F]);
		p += 3;
		len -= 3;
	}

	if (len == 1)
	{
		BYTE c1 = p[0];
		s.push_back(BASE64_CHAR_MAP[c1 >> 2]);
		s.push_back(BASE64_CHAR_MAP[(c1 & 3) << 4]);
		s.push_back('=');
		s.push_back('=');
	}
	else if (len == 2)
	{
		BYTE c1 = p[0], c2 = p[1];
		s.push_back(BASE64_CHAR_MAP[c1 >> 2]);
		s.push_back(BASE64_CHAR_MAP[((c1 & 3) << 4) | (c2 >> 4)]);
		s.push_back(BASE64_CHAR_MAP[(c2 & 15) << 2]);
		s.push_back('=');
	}
	return stringx(s);
}

ref<ByteArray> Base64Decode(stringx data)
{
	return NULL;
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
