#include "stdafx.h"
#include "../../../test_support.h"
#include "../../../../include/cdecegg/intern/algorithm/search.h"

using namespace cdec_egg;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

template<typename _Ty>
struct PlainArray
{
	const _Ty*	m_data;
	int			m_count;

	PlainArray(const _Ty* data, int count): m_data(data), m_count(count) {}
	int		Count() { return m_count; }
	const _Ty&	operator[](int index) { return m_data[index]; }
};

template<typename _Ty>
struct Cmp
{
	int	operator()(const _Ty& lhs, const _Ty& rhs)
	{
		if (lhs < rhs)
			return -1;
		else if (lhs == rhs)
			return 0;
		else
			return 1;
	}
};

// -------------------------------------------------------------------------- //

class TestSearch: public UnitTestSuite
{
	UNITTEST_SUITE(TestSearch)
		UNITTEST_METHOD(testBinarySearchGE)
		UNITTEST_METHOD(testBinarySearchLE)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testBinarySearchGE()
	{
		int	arr1[] = { 1, 3, 5 };
		PlainArray<int>	arr1s(arr1, sizeof(arr1) / sizeof(int));
		std::pair<int, bool> r = SearchSortedArrayGE(0, arr1s, Cmp<int>());
		UNITTEST_ASSERT(r.first == 0 && !r.second);

		r = SearchSortedArrayGE(1, arr1s, Cmp<int>());
		UNITTEST_ASSERT(r.first == 0 && r.second);

		r = SearchSortedArrayGE(2, arr1s, Cmp<int>());
		UNITTEST_ASSERT(r.first == 1 && !r.second);

		r = SearchSortedArrayGE(3, arr1s, Cmp<int>());
		UNITTEST_ASSERT(r.first == 1 && r.second);

		r = SearchSortedArrayGE(4, arr1s, Cmp<int>());
		UNITTEST_ASSERT(r.first == 2 && !r.second);

		r = SearchSortedArrayGE(5, arr1s, Cmp<int>());
		UNITTEST_ASSERT(r.first == 2 && r.second);

		r = SearchSortedArrayGE(6, arr1s, Cmp<int>());
		UNITTEST_ASSERT(r.first == 3 && !r.second);

		for (int i = 0; i <= 8; ++i)
			verifySearchSortedArrayGE(i);
	}

	void verifySearchSortedArrayGE(int count)
	{
		int	parr[] = { 1, 3, 5, 7, 9, 11, 13, 15 };
		PlainArray<int>	arr3(parr, count);
		int _seed = 0;
		bool _flag = false;
		for (int i = 0; i <= count * 2; ++i)
		{
			std::pair<int, bool> r = SearchSortedArrayGE(i, arr3, Cmp<int>());
			UNITTEST_ASSERT(r.first == _seed && r.second == _flag);
			_flag = !_flag;
			if (!_flag)
				++_seed;
		}		
	}

	void testBinarySearchLE()
	{
		for (int i = 0; i <= 8; ++i)
			verifySearchSortedArrayLE(i);
	}

	void verifySearchSortedArrayLE(int count)
	{
		int	parr[] = { 1, 3, 5, 7, 9, 11, 13, 15 };
		PlainArray<int>	arr3(parr, count);
		int _seed = -1;
		bool _flag = false;
		for (int i = 0; i <= count * 2; ++i)
		{
			std::pair<int, bool> r = SearchSortedArrayLE(i, arr3, Cmp<int>());
			UNITTEST_ASSERT(r.first == _seed && r.second == _flag);
			_flag = !_flag;
			if (_flag)
				++_seed;
		}		
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestSearch);

// -------------------------------------------------------------------------- //
#endif
