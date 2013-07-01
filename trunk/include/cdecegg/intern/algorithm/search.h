#pragma once

CDEC_EGG_NS_BEGIN
// -------------------------------------------------------------------------- //

// Find the first element not less than the given UID
// Return:
//		If found, a pair of <index of element, true> is returned
//		If not found, a pair of <first element greater, false> is returned
// The list by _Ar must be a sorted list, in a same comparer of _Cmp
// If multiple copies of an element exist in the list, any one of them may be returned
template<typename _Ty, typename _Ar, typename _Cmp>
std::pair<int, bool> SearchSortedArrayGE(_Ty val, _Ar& ar, _Cmp cmp)
{
	int	l = 0, h = ar.Count();
	while (l < h)	// [l, h)
	{
		int m = (l + h) >> 1;
		_Ty val_t = ar[m];
		switch (cmp(val_t, val))
		{
		case 0:			// Exactly this one
			return std::pair<int, bool>(m, true);
		case 1:			// itm[m] > x
			h = m;		// [l, m)
			break;
		default:		// itm[m] < x
			l = m + 1;	// [m + 1, h)
			break;
		}
	}
	ASSERT(l == h);
	return std::pair<int, bool>(l, false);
}

// Find the first element not greater than the given UID
// Return:
//		If found, a pair of <index of element, true> is returned
//		If not found, a pair of <last element less, false> is returned
// The list by _Ar must be a sorted list, in a same comparer of _Cmp
// If multiple copies of an element exist in the list, any one of them may be returned
template<typename _Ty, typename _Ar, typename _Cmp>
std::pair<int, bool> SearchSortedArrayLE(_Ty val, _Ar& ar, _Cmp cmp)
{
	std::pair<int, bool> r = SearchSortedArrayGE(val, ar, cmp);
	if (!r.second)
		--r.first;
	return r;
}

// -------------------------------------------------------------------------- //
CDEC_EGG_NS_END
