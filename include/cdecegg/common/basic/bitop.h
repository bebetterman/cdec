// -------------------------------------------------------------------------- //
//	文件名		：	kfc/rtl/lite_bitop.h
//	创建者		：	杨钢
//	创建时间	：	2007-5-9 13:34:57
//	功能描述	：	
//
// -------------------------------------------------------------------------- //

#pragma once

CDEC_EGG_NS_BEGIN

// -------------------------------------------------------------------------- //
// 逻辑异或和同或
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
// 位操作函数，请只用于整数类型
// -------------------------------------------------------------------------- //

// 设置对应的位
template<typename T> inline void SetBitX(T& value, T bit)
{
	value |= bit;
}

inline void SetBit(UINT32& value, UINT32 bit) { return SetBitX<UINT32>(value, bit); }
inline void SetBit64(UINT64& value, UINT64 bit) { return SetBitX<UINT64>(value, bit); }

// 设置对应的位
template<typename T> inline void SetBitX(T& value, T bit, bool bSet)
{
	value = (value & ~bit) | (bSet ? bit : 0);
}

inline void SetBit(UINT32& value, UINT32 bit, bool f) { return SetBitX<UINT32>(value, bit, f); }
inline void SetBit64(UINT64& value, UINT64 bit, bool f) { return SetBitX<UINT64>(value, bit, f); }

// 清除对应的位
template<typename T> inline void ClearBitsX(T value, T bit)
{
	value &= ~bit;
}

inline void ClearBits(UINT32& value, UINT32 bit) { return ClearBitsX<UINT32>(value, bit); }
inline void ClearBits64(UINT64& value, UINT64 bit) { return ClearBitsX<UINT64>(value, bit); }

// 获取对应的位
template<typename T> inline T GetMaskBitsX(T value, T mask)
{
	return value & mask;
}

inline UINT32 GetMaskBits(UINT32 value, UINT32 mask) { return GetMaskBitsX<UINT32>(value, mask); }
inline UINT64 GetMaskBits64(UINT64 value, UINT64 mask) { return GetMaskBitsX<UINT64>(value, mask); }

// 设置对应的位
/*
@fn dwMask
@brief
	根据 dwRef 更新 dwTag 的相应标记位
@arg [ref] dwTag	更新的目标
@arg [in] dwRef		更新的参考源
@arg [in] dwMask	需要更新的标记位
@return
@remark
	本函数将源上的指定位复制到目标上，例如：
	dwTag = 01010101，dwRef = 10101010，dwMask = 110100，那么，更新的结果 dwTag = 01100001
	计算过程如下：
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

// 检查对应位是否置上，如果 mask 有多位，如果其中任何一位被置上，则返回 true
template<typename T> inline bool IsBitUsedX(T value, T mask)
{
	return (value & mask) != 0;
}

inline bool IsBitUsed(UINT32 value, UINT32 mask) { return IsBitUsedX<UINT32>(value, mask); }
inline bool IsBitUsed64(UINT64 value, UINT64 mask) { return IsBitUsedX<UINT64>(value, mask); }

/*
@fn UpdateBitAccording
@brief
	如果 refer 的 refer_mask 位为 0（或 1），则将 value 的 value_mask 位设置为 0（或 1）。
@arg [ref] value		更新的目标
@arg [in] value_mask	更新目标的标记位
@arg [in] refer			更新的参考源
@arg [in] refer_mask	参考源的标记位
@return
@remark
	使用本函数时注意一次仅对一位进行操作。例如：
	value = 0000, value_mask = 1000, refer = 1111, refer_mask = 0001，则结果为 value = 1000
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
	获取两个数的指定位是否相同
@arg [in] value1	待比较的数
@arg [in] value2	待比较的数
@arg [in] mask	指定待比较位的掩码
@return
	指定位相同返回 true。
@remark
	此函数用于比较 dwMask 指定的位上，两个数是否相同。例如：
	两个数为 10100100, 01101000，dwMask = 00100011m。
@*/
template<typename T> inline bool IsBitIdenticalX(T value1, T value2, T mask)
{
	return ((value1 ^ value2) & mask) == 0;
}

inline bool IsBitIdentical(UINT32 value1, UINT32 value2, UINT32 mask) { return IsBitIdenticalX<UINT32>(value1, value2, mask); }
inline bool IsBitIdentical64(UINT64 value1, UINT64 value2, UINT64 mask) { return IsBitIdenticalX<UINT64>(value1, value2, mask); }

// -------------------------------------------------------------------------- //

CDEC_EGG_NS_END
