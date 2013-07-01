// -------------------------------------------------------------------------- //
//	文件名		：	bitarray.h
//	创建者		：	杨钢
//	创建时间	：	2006-4-10 16:27:36
//	功能描述	：	Bit Array
//
// -------------------------------------------------------------------------- //

// Bit array 是一个固定大小的位数组，之所以专门搞一个这个而不是使用 vector<bool>
// 是基于效率考虑。

#pragma once

CDEC_EGG_NS_BEGIN

// -------------------------------------------------------------------------- //
// 固定大小的
// -------------------------------------------------------------------------- //

typedef UINT	BitOrgType;

const int BitOrgByteSize = sizeof(BitOrgType);
const int BitOrgBitSize = BitOrgByteSize * 8;

const BitOrgType BOT_FULLTRUE = DW_FULLBIT;

// 注意：Memory Provider 要求是共享型

template<typename _MP>
class bit_array
{
public:
	typedef _MP		memory_provider;

protected:
	memory_provider	m_mp;
	UINT			m_bitSize;
	UINT			m_cbBuf;	// 稍显浪费
	BitOrgType*		m_pBuf;

public:
	bit_array(UINT bitSize, memory_provider mp = memory_provider());
	bit_array(const bit_array<_MP>& r);
	~bit_array() { clear(); }

	// Warning: former elements are cleared
	void reset(UINT bitSize);
	void clear();

	UINT size() const { return m_bitSize; }

	void set_all(bool f)
	{
		memset(m_pBuf, f ? 0xFF : 0, m_cbBuf);
	}

	void set(int index, bool bv)
	{
		BitOrgType* pdwValue;
		BitOrgType	dwMask;
		cast_address(index, pdwValue, dwMask);
		SetBit(*pdwValue, dwMask, bv);
	}

	bool get(int index) const
	{
		BitOrgType* pdwValue;
		BitOrgType	dwMask;
		cast_address(index, pdwValue, dwMask);
		return IsBitUsed(*pdwValue, dwMask);
	}

	bool full_false() const;
	bool full_true() const;

	int first_true() const;
	int first_false() const;

	void* get_buffer() { return m_pBuf; }
	const void* get_buffer() const { return m_pBuf; }

protected:
	void alloc_space()
	{
		ASSERT(m_bitSize > 0);
		m_cbBuf = (m_bitSize + BitOrgBitSize - 1) / BitOrgBitSize * BitOrgByteSize;
		m_pBuf = static_cast<BitOrgType*>( m_mp.Alloc(m_cbBuf) );
	}

	void cast_address(int bitIndex, BitOrgType*& pdwValue, BitOrgType& dwMask) const
	{
		ASSERT(bitIndex >= 0 && static_cast<UINT>(bitIndex) < m_bitSize);
		// 不知道编译器此处优化效果如何
		int		dwIndex = bitIndex / BitOrgBitSize;
		int		dwResidue = bitIndex % BitOrgBitSize;
		pdwValue = m_pBuf + dwIndex;
		dwMask = GetBitMask(dwResidue);
	}
};

// -------------------------------------------------------------------------- //

template<typename _MP>
bit_array<_MP>::bit_array(UINT bitSize, memory_provider mp):
	m_bitSize(bitSize), m_mp(mp)
{
	alloc_space();
	memset(m_pBuf, 0, m_cbBuf);
}

template<typename _MP>
bit_array<_MP>::bit_array(const bit_array<_MP>& r):
	m_bitSize(r.m_bitSize), m_mp(r.m_mp)
{
	alloc_space();
	memcpy(m_pBuf, r.m_pBuf, m_cbBuf);
}

template<typename _MP>
void bit_array<_MP>::clear()
{
	m_mp.Free(m_pBuf);
	m_pBuf = NULL;
	m_bitSize = 0;
}

template<typename _MP>
void bit_array<_MP>::reset(UINT bitSize)
{
	clear();
	m_bitSize = bitSize;
	alloc_space();
	memset(m_pBuf, 0, m_cbBuf);
}

template<typename _MP>
bool bit_array<_MP>::full_false() const
{
	int		nFullDw = m_bitSize / BitOrgBitSize;
	int		bitResidue = m_bitSize % BitOrgBitSize;

	BitOrgType*	p = m_pBuf;
	if (nFullDw > 0)
	{
		for (int i = 0; i < nFullDw; ++i)
		{
			if (*p++ != 0)
				return false;
		}
	}

	if (bitResidue > 0)
		return !IsBitUsed(*p, GetAccuLow(bitResidue));
	else
		return true;
}

template<typename _MP>
bool bit_array<_MP>::full_true() const
{
	int		nFullDw = m_bitSize / BitOrgBitSize;
	int		bitResidue = m_bitSize % BitOrgBitSize;

	BitOrgType*	p = m_pBuf;
	if (nFullDw > 0)
	{
		for (int i = 0; i < nFullDw; ++i)
		{
			if (*p++ != BOT_FULLTRUE)
				return false;
		}
	}

	if (bitResidue > 0)
		return GetMaskBits(*p, GetAccuLow(bitResidue)) == GetAccuLow(bitResidue);
	else
		return true;
}

template<typename _MP>
int bit_array<_MP>::first_false() const
{
	UINT		i;
	BitOrgType*	p = m_pBuf;

	// 先找有空位的双字
	for (i = 0; i < m_cbBuf / sizeof(BitOrgType); ++i, ++p)
	{
		if (*p != BOT_FULLTRUE)
			break;
	}
	if (i >= m_cbBuf / sizeof(BitOrgType))
		return -1;	// 找不到

	// 再找到空位
	ASSERT(*p != BOT_FULLTRUE);
	i = i * BitOrgBitSize + GetLowFalseBit(*p);

	return i;
}

// -------------------------------------------------------------------------- //
// 可变大小的
// -------------------------------------------------------------------------- //

// 请使用 std::vector<bool>

// -------------------------------------------------------------------------- //

CDEC_EGG_NS_END
