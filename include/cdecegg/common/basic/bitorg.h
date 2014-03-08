// -------------------------------------------------------------------------- //
//	�ļ���		��	alg/bit_org.h
//	������		��	���
//	����ʱ��	��	2007-3-12 10:48:25
//	��������	��	����λ��������
//
// -------------------------------------------------------------------------- //

// ���ļ��� Kfc/Alg �� AlgDiag ����
//

#pragma once

CDEC_EGG_NS_BEGIN
// -------------------------------------------------------------------------- //
// DWORD λ��������
// -------------------------------------------------------------------------- //

// �������к��������λ������Ϊ 0��DWORD ���λ����Ϊ 31
// ���磺1 �ĵ� 0 λΪ 1������λΪ 0
// ��Ӧ�ģ�0x80000000 �ĵ� 31 λΪ 1������λΪ 0

const UINT	DW_FULLBIT		= 0xFFFFFFFF,
			DW_HIGHESTBIT	= 0x80000000;

// ���� x �Ӹ�λ��ʼ�ĵ�һ��ֵΪ 1 ��λ
// �������λ��Ϊ 0������ 0
CDECEGGEXPORT int GetHighTrueBit(UINT x);

// ���� x �ӵ�λ��ʼ�ĵ�һ��ֵΪ 1 ��λ
// �������λ��Ϊ 0������ 31
CDECEGGEXPORT int GetLowTrueBit(UINT x);

// ���� x �Ӹ�λ��ʼ�ĵ�һ��ֵΪ 0 ��λ
// �������λ��Ϊ 1������ 0
inline int GetHighFalseBit(UINT x)
{
	return GetHighTrueBit(~x);
}

// ���� x �ӵ�λ��ʼ�ĵ�һ��ֵΪ 0 ��λ
// �������λ��Ϊ 1������ 31
inline int GetLowFalseBit(UINT x)
{
	return GetLowTrueBit(~x);
}

// ��ȡ�� i λ������λ�����磬�� 3 λ������λΪ 0x8
CDECEGGEXPORT UINT GetBitMask(int i);

// ��ȡ�� i λΪ 1�������λΪ 0 ��ֵ������
// i = 6 ʱ�����Ϊ 0xFC000000
CDECEGGEXPORT UINT GetAccuHigh(int i);

// ��ȡ�� i λΪ 1�������λΪ 0 ��ֵ������
// i = 6 ʱ�����Ϊ 0x0000003F
CDECEGGEXPORT UINT GetAccuLow(int i);

// ��ȡ�����ڸ����� x ������ 2^n
// ����������Ϊ 0���򷵻� 0
CDECEGGEXPORT UINT GetMaxPow2LE(UINT x);

// ��ȡ��С�ڸ����� x ����С�� 2^n
// ����������Ϊ 0���򷵻� 1
// ������������λΪ 1������ 0x80000000�����򷵻� 0
inline UINT GetMinPow2GE(UINT x)
{
	if (x == 0)
		return 1;

	DWORD r = GetMaxPow2LE(x);
	ASSERT(r <= x);
	return r < x ? r + r : r;
}

// -------------------------------------------------------------------------- //

// ��ȡ��������λ�� 1 �ĸ���
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
