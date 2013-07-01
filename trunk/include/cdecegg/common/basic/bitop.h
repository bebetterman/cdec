// -------------------------------------------------------------------------- //
//	�ļ���		��	kfc/rtl/lite_bitop.h
//	������		��	���
//	����ʱ��	��	2007-5-9 13:34:57
//	��������	��	
//
// -------------------------------------------------------------------------- //

#pragma once

CDEC_EGG_NS_BEGIN

// -------------------------------------------------------------------------- //
// �߼�����ͬ��
// -------------------------------------------------------------------------- //

inline bool LOGXOR(bool a, bool b)
{
	return a != b;
}

inline bool LOGXOR(int a, int b)
{
	return (a != 0) != (b != 0);
}

inline bool LOGEOR(bool a, bool b)
{
	return a == b;
}

inline bool LOGEOR(int a, int b)
{
	return (a != 0) == (b != 0);
}

// -------------------------------------------------------------------------- //
// λ������������ֻ������������
// -------------------------------------------------------------------------- //

// ���ö�Ӧ��λ
template<typename T> inline void SetBitX(T& value, T bit)
{
	value |= bit;
}

inline void SetBit(UINT32& value, UINT32 bit) { return SetBitX<UINT32>(value, bit); }
inline void SetBit64(UINT64& value, UINT64 bit) { return SetBitX<UINT64>(value, bit); }

// ���ö�Ӧ��λ
template<typename T> inline void SetBitX(T& value, T bit, bool bSet)
{
	value = (value & ~bit) | (bSet ? bit : 0);
}

inline void SetBit(UINT32& value, UINT32 bit, bool f) { return SetBitX<UINT32>(value, bit, f); }
inline void SetBit64(UINT64& value, UINT64 bit, bool f) { return SetBitX<UINT64>(value, bit, f); }

// �����Ӧ��λ
template<typename T> inline void ClearBitsX(T value, T bit)
{
	value &= ~bit;
}

inline void ClearBits(UINT32& value, UINT32 bit) { return ClearBitsX<UINT32>(value, bit); }
inline void ClearBits64(UINT64& value, UINT64 bit) { return ClearBitsX<UINT64>(value, bit); }

// ��ȡ��Ӧ��λ
template<typename T> inline T GetMaskBitsX(T value, T mask)
{
	return value & mask;
}

inline UINT32 GetMaskBits(UINT32 value, UINT32 mask) { return GetMaskBitsX<UINT32>(value, mask); }
inline UINT64 GetMaskBits64(UINT64 value, UINT64 mask) { return GetMaskBitsX<UINT64>(value, mask); }

// ���ö�Ӧ��λ
/*
@fn dwMask
@brief
	���� dwRef ���� dwTag ����Ӧ���λ
@arg [ref] dwTag	���µ�Ŀ��
@arg [in] dwRef		���µĲο�Դ
@arg [in] dwMask	��Ҫ���µı��λ
@return
@remark
	��������Դ�ϵ�ָ��λ���Ƶ�Ŀ���ϣ����磺
	dwTag = 01010101��dwRef = 10101010��dwMask = 110100����ô�����µĽ�� dwTag = 01100001
	����������£�
	dwTag  =(0 1)0 1(0)1(0 1)
	dwRef  = 1 0(1 0)1(0)1 0
	dwMask = - - 1 1 - 1 - -
	-------------------------
	         0 1 1 0 0 0 0 1
@*/
template<typename T> inline void SetMaskBitsX(T& value, T mask, T mask_value)
{
	value = (value & ~mask) | (mask_value & mask);
}

inline void SetMaskBits(UINT32& value, UINT32 mask, UINT32 mask_value) { SetMaskBitsX<UINT32>(value, mask, mask_value); }
inline void SetMaskBits64(UINT64& value, UINT64 mask, UINT64 mask_value) { SetMaskBitsX<UINT64>(value, mask, mask_value); }

// ����Ӧλ�Ƿ����ϣ���� mask �ж�λ����������κ�һλ�����ϣ��򷵻� true
template<typename T> inline bool IsBitUsedX(T value, T mask)
{
	return (value & mask) != 0;
}

inline bool IsBitUsed(UINT32 value, UINT32 mask) { return IsBitUsedX<UINT32>(value, mask); }
inline bool IsBitUsed64(UINT64 value, UINT64 mask) { return IsBitUsedX<UINT64>(value, mask); }

/*
@fn UpdateBitAccording
@brief
	��� refer �� refer_mask λΪ 0���� 1������ value �� value_mask λ����Ϊ 0���� 1����
@arg [ref] value		���µ�Ŀ��
@arg [in] value_mask	����Ŀ��ı��λ
@arg [in] refer			���µĲο�Դ
@arg [in] refer_mask	�ο�Դ�ı��λ
@return
@remark
	ʹ�ñ�����ʱע��һ�ν���һλ���в��������磺
	value = 0000, value_mask = 1000, refer = 1111, refer_mask = 0001������Ϊ value = 1000
@*/
template<typename T> inline void UpdateBitAccordingX(T& value, T value_mask, T refer, T refer_mask)
{
	if (refer & refer_mask)
		value |= value_mask;
	else
		value &= ~value_mask;
}

inline void UpdateBitAccording(UINT32& value, UINT32 value_mask, UINT32 refer, UINT32 refer_mask) { UpdateBitAccordingX<UINT32>(value, value_mask, refer, refer_mask); }
inline void UpdateBitAccording64(UINT64& value, UINT64 value_mask, UINT64 refer, UINT64 refer_mask) { UpdateBitAccordingX<UINT64>(value, value_mask, refer, refer_mask); }

/*
@fn IsBitIdentical
@brief
	��ȡ��������ָ��λ�Ƿ���ͬ
@arg [in] value1	���Ƚϵ���
@arg [in] value2	���Ƚϵ���
@arg [in] mask	ָ�����Ƚ�λ������
@return
	ָ��λ��ͬ���� true��
@remark
	�˺������ڱȽ� dwMask ָ����λ�ϣ��������Ƿ���ͬ�����磺
	������Ϊ 10100100, 01101000��dwMask = 00100011m��
@*/
template<typename T> inline bool IsBitIdenticalX(T value1, T value2, T mask)
{
	return ((value1 ^ value2) & mask) == 0;
}

inline bool IsBitIdentical(UINT32 value1, UINT32 value2, UINT32 mask) { return IsBitIdenticalX<UINT32>(value1, value2, mask); }
inline bool IsBitIdentical64(UINT64 value1, UINT64 value2, UINT64 mask) { return IsBitIdenticalX<UINT64>(value1, value2, mask); }

// -------------------------------------------------------------------------- //

CDEC_EGG_NS_END
