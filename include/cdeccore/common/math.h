#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

class CDECCOREEXPORT Math: public Object
{
	DECLARE_REF_CLASS(Math)

public:
	template<typename T>
	static T Min(T a, T b) { return a <= b ? a : b; }

	template<typename T>
	static T Max(T a, T b) { return a >= b ? a : b; }

	static bool Equal(double a, double b)
	{
		if (a == b)
			return true;

		double delta = a - b, sum = a + b;
		if (sum != 0)
			delta /= sum;
		if (delta < 0)
			delta = -delta;
		return delta < 1e-14;
	}
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
