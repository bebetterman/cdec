// -------------------------------------------------------------------------- //
//	文件名		：	alg/bit_org.h
//	创建者		：	杨钢
//	创建时间	：	2007-3-12 10:48:25
//	功能描述	：	基本位操作例程
//
// -------------------------------------------------------------------------- //

// 本文件被 Kfc/Alg 与 AlgDiag 共享
//

#pragma once

CDEC_EGG_NS_BEGIN
// -------------------------------------------------------------------------- //
// DWORD 位操作函数
// -------------------------------------------------------------------------- //

// 以下所有函数，最低位的索引为 0，DWORD 最高位索引为 31
// 例如：1 的第 0 位为 1，其他位为 0
// 相应的，0x80000000 的第 31 位为 1，其他位为 0

const UINT	DW_FULLBIT		= 0xFFFFFFFF,
			DW_HIGHESTBIT	= 0x80000000;

// 返回 x 从高位开始的第一个值为 1 的位
// 如果所有位都为 0，返回 0
CDECEGGEXPORT int GetHighTrueBit(UINT x);

// 返回 x 从低位开始的第一个值为 1 的位
// 如果所有位都为 0，返回 31
CDECEGGEXPORT int GetLowTrueBit(UINT x);

// 返回 x 从高位开始的第一个值为 0 的位
// 如果所有位都为 1，返回 0
inline int GetHighFalseBit(UINT x)
{
	return GetHighTrueBit(~x);
}

// 返回 x 从低位开始的第一个值为 0 的位
// 如果所有位都为 1，返回 31
inline int GetLowFalseBit(UINT x)
{
	return GetLowTrueBit(~x);
}

// 获取第 i 位的屏蔽位，例如，第 3 位的屏蔽位为 0x8
CDECEGGEXPORT UINT GetBitMask(int i);

// 获取高 i 位为 1，其余低位为 0 的值，例如
// i = 6 时，结果为 0xFC000000
CDECEGGEXPORT UINT GetAccuHigh(int i);

// 获取低 i 位为 1，其余高位为 0 的值，例如
// i = 6 时，结果为 0x0000003F
CDECEGGEXPORT UINT GetAccuLow(int i);

// 获取不大于给定的 x 的最大的 2^n
// 如果传入参数为 0，则返回 0
CDECEGGEXPORT UINT GetMaxPow2LE(UINT x);

// 获取不小于给定的 x 的最小的 2^n
// 如果传入参数为 0，则返回 1
// 如果传入参数高位为 1（大于 0x80000000），则返回 0
inline UINT GetMinPow2GE(UINT x)
{
	if (x == 0)
		return 1;

	DWORD r = GetMaxPow2LE(x);
	ASSERT(r <= x);
	return r < x ? r + r : r;
}

// -------------------------------------------------------------------------- //

// 获取二进制数位中 1 的个数
inline UINT GetBitCount(UINT value)
{
	value = ((value >> 1) & 0x55555555) + (value & 0x55555555);	// 1 * 32 -> 2 * 16
	value = ((value >> 2) & 0x33333333) + (value & 0x33333333);	// 2 * 16 -> 4 * 8
	value = ((value >> 4) & 0x0F0F0F0F) + (value & 0x0F0F0F0F); // 4 * 8 -> 8 * 4
	value = ((value >> 8) & 0x00FF00FF) + (value & 0x00FF00FF); // 8 * 4 -> 4 * 2
	return (value >> 16) + (value & 0xFFFF);
}

inline UINT RoundUp2(UINT value, UINT n)
{
	ASSERT(GetBitCount(n) == 1);
	--n;	// Get mask bits
	return (value + n) & ~n;
}

inline UINT RoundUpN(UINT value, UINT n)
{
	ASSERT(n != 0);
	value += n - 1;
	return value - (value % n);
}

// -------------------------------------------------------------------------- //
CDEC_EGG_NS_END
