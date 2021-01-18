#pragma once
#include "General.h"
#include <functional>

namespace Prism::Math
{
	inline prdecimal Integrate(std::function<prdecimal(prdecimal)> f, prdecimal start, prdecimal end, prdecimal step)
	{
		float sum = 0;
		while (start < end)
		{
			sum += f(start + step) * step;
			start += step;
		}

		return sum;
	}
}
