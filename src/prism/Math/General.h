#pragma once

#include <cmath>

namespace Prism::Math
{
#ifdef PR_MATH_DOUBLES
	using prdecimal = double;
#else
	using prdecimal = float;
#endif

	inline prdecimal FastACos(float x)
	{
		constexpr auto a = static_cast<prdecimal>(-0.69813170079773212);
		constexpr auto b = static_cast<prdecimal>(0.87266462599716477);
		constexpr auto c = static_cast<prdecimal>(1.5707963267948966);
		return (a * x * x - b) * x + c;
	}
}
