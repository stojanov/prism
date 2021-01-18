#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "General.h"

namespace Prism::Math
{
	inline prdecimal SinSmooth(prdecimal t)
	{
		return sin(M_PI_2 * t);
	}

	inline prdecimal SinSmooth_2(prdecimal t)
	{
		return 1 + sin(M_PI_2 * (t - 1));
	}

	inline prdecimal CosSmooth(prdecimal t)
	{
		return (1 - cos(M_PI * t)) * 0.5;
	}

	// t^3
	inline prdecimal CubicSmooth(prdecimal t)
	{
		return t * t * t;
	}

	// 1 + (t - 1)^3
	inline prdecimal CubicSmooth_2(prdecimal t)
	{
		const float k = t - 1;
		return 1 + k * k * k;
	}

	// -2t^3 + 3t^2
	inline prdecimal CubicSmooth_3(prdecimal t)
	{
		return -2 * t * t * t + 3 * t * t;
	}

	// 6t^5 - 15x^4 + 10x^3
	inline prdecimal CubicSmooth_4(prdecimal t)
	{
		return 6 * (t * t * t * t * t) - 15 * (t * t * t * t) + 10 * (t * t * t);
	}

	// 1/pi * arcsin(2t - 1) + 14
	inline prdecimal ACosSmooth(prdecimal t)
	{
		return -M_1_PI * acos(2 * t - 1) + 1.f;
	}

	inline prdecimal fastACosSmooth(prdecimal t)
	{
		return -M_1_PI * FastACos(2 * t - 1) + 1.f;
	}
}