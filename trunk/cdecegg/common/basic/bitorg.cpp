// -------------------------------------------------------------------------- //
//	文件名		：	alg/bit_org_impl.h
//	创建者		：	杨钢
//	创建时间	：	2007-3-12 10:52:39
//	功能描述	：	
//
// -------------------------------------------------------------------------- //

#include <stdafx.h>

CDEC_EGG_NS_BEGIN
// -------------------------------------------------------------------------- //

const UINT	DW_HIBITFLT_1 = 0xFFFF0000,
			DW_HIBITFLT_2 = 0xFF00FF00,
			DW_HIBITFLT_3 = 0xF0F0F0F0,
			DW_HIBITFLT_4 = 0xCCCCCCCC,
			DW_HIBITFLT_5 = 0xAAAAAAAA;
const UINT	DW_LOBITFLT_1 = 0x0000FFFF,
			DW_LOBITFLT_2 = 0x00FF00FF,
			DW_LOBITFLT_3 = 0x0F0F0F0F,
			DW_LOBITFLT_4 = 0x33333333,
			DW_LOBITFLT_5 = 0x55555555;

UINT	DW_BITMASK[32] = {
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
};

UINT	DW_ACCU_LO[33] = {	0,
	0x00000001, 0x00000003, 0x00000007, 0x0000000F,
	0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF,
	0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF,
	0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF,
	0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF,
	0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
	0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF,
	0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
};

UINT	DW_ACCU_HI[33] = {	0,
	0x80000000, 0xC0000000, 0xE0000000, 0xF0000000,
	0xF8000000, 0xFC000000, 0xFE000000, 0xFF000000,
	0xFF800000, 0xFFC00000, 0xFFE00000, 0xFFF00000,
	0xFFF80000, 0xFFFC0000, 0xFFFE0000, 0xFFFF0000,
	0xFFFF8000, 0xFFFFC000, 0xFFFFE000, 0xFFFFF000,
	0xFFFFF800, 0xFFFFFC00, 0xFFFFFE00, 0xFFFFFF00,
	0xFFFFFF80, 0xFFFFFFC0, 0xFFFFFFE0, 0xFFFFFFF0,
	0xFFFFFFF8, 0xFFFFFFFC, 0xFFFFFFFE, 0xFFFFFFFF,
};

// -------------------------------------------------------------------------- //

CDECEGGEXPORT int GetHighTrueBit(UINT x)
{
#ifdef _DEBUG
	// 用简单的算法算出一个结果作为验证
	UINT _x = x != 0 ? x : 1;
	int _i = 0;
	while ((_x & DW_HIGHESTBIT) == 0)
	{
		++_i;
		_x <<= 1;
	}
	_i = 31 - _i;
#endif

	int i = 0;

	if (x & DW_HIBITFLT_1)	// 判断高 16 位
	{
		x &= DW_HIBITFLT_1;
		i = 16;
	}
	if (x & DW_HIBITFLT_2)	// 判断某 16 位中的高 8 位
	{
		x &= DW_HIBITFLT_2;
		i += 8;
	}
	if (x & DW_HIBITFLT_3)	// 判断某 8 位中的高 4 位
	{
		x &= DW_HIBITFLT_3;
		i += 4;
	}
	if (x & DW_HIBITFLT_4)	// 判断某 4 位中的高 2 位
	{
		x &= DW_HIBITFLT_4;
		i += 2;
	}
	if (x & DW_HIBITFLT_5)	// 判断某 2 位中的高位
	{
		++i;
	}

	ASSERT(i >= 0 && i < 32 && i == _i);
	return i;
}

CDECEGGEXPORT int GetLowTrueBit(UINT x)
{
#ifdef _DEBUG
	// 用简单的算法算出一个结果作为验证
	UINT _x = x != 0 ? x : DW_HIGHESTBIT;
	int _i = 0;
	while ((_x & 1) == 0)
	{
		++_i;
		_x >>= 1;
	}
#endif

	int i = 0;

	if (x & DW_LOBITFLT_1)	// 判断低 16 位
		x &= DW_LOBITFLT_1;
	else
		i = 16;

	if (x & DW_LOBITFLT_2)	// 判断某 16 位中的低 8 位
		x &= DW_LOBITFLT_2;
	else
		i += 8;
	
	if (x & DW_LOBITFLT_3)	// 判断某 8 位中的低 4 位
		x &= DW_LOBITFLT_3;
	else
		i += 4;

	if (x & DW_LOBITFLT_4)	// 判断某 4 位中的低 2 位
		x &= DW_LOBITFLT_4;
	else
		i += 2;

	if (!(x & DW_LOBITFLT_5))	// 判断某 2 位中的低位
	{
		++i;
	}

	ASSERT(i >= 0 && i < 32 && i == _i);
	return i;
}

CDECEGGEXPORT UINT GetBitMask(int i)
{
	ASSERT(i >= 0 && i < 32);
	return DW_BITMASK[i];
}

CDECEGGEXPORT UINT GetAccuHigh(int i)
{
	ASSERT(i >= 0 && i <= 32);
	return DW_ACCU_HI[i];
}

CDECEGGEXPORT UINT GetAccuLow(int i)
{
	ASSERT(i >= 0 && i <= 32);
	return DW_ACCU_LO[i];
}

CDECEGGEXPORT UINT GetMaxPow2LE(UINT x)
{
	if (x == 0)
		return 0;

	int i = GetHighTrueBit(x);
	return DW_BITMASK[i];
}

// -------------------------------------------------------------------------- //
CDEC_EGG_NS_END
