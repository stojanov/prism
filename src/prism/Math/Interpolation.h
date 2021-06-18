#pragma once

#include <glm.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "General.h"

namespace Prism::Math
{
	template<typename T>
	constexpr prdecimal LinearInterpolate(T a, T b, T x)
	{
		return  a * (1 - x) + b * x;
	}

	template<float(*f)(float), typename T>
	constexpr prdecimal SmoothLinearInterpolate(T a, T b, T x)
	{
		return f(LinearInterpolate(a, b, x));
	}

	template<typename T>
	constexpr prdecimal CosineInterpolate(T a, T b, T x)
	{
		auto ft = x * M_PI;
		auto f = (1 - cos(ft)) * 0.5f;
		return  a * (1 - f) + b * f;
	}

	template<typename T>
	constexpr prdecimal CubicInterpolate(T v0, T v1, T v2, T v3, T val)
	{
		const float P = (v3 - v2) - (v0 - v1);
		const float Q = (v0 - v1) - P;
		const float R = v2 - v0;

		return P * val * val * val + Q * val * val + R * val + v1;
	}

	template<typename T, typename U>
	constexpr prdecimal LinearTranslate(T a1, T b1, U a2, U b2, T x)
	{
		return a2 + (b2 - a2) * (1.f * (x - a1) / (b1 - a1));
	}

	template<float(*f)(float), typename T, typename U>
	constexpr auto SmoothLinearTranslate(T a1, T b1, U a2, U b2, T x)
	{
		return (a2 + (b2 - a2) * f(LinearTranslate(a1, b1, 0.0f, 1.0f, x)));
	}
	// TODO: Check if maths is correct
	constexpr auto BilinearInterpolate(
		const glm::vec2& a1, const glm::vec2& a2,
		const glm::vec2& b1, const glm::vec2& b2, 
		const glm::vec2& t)
	{
		auto x1 = LinearInterpolate(a1.x, a2.x, t.x);
		auto x2 = LinearInterpolate(b1.x, b2.x, t.x);
		
		return LinearInterpolate(x1, x2, t.y);
	}

	constexpr auto CosineBilinearInterpolate(
		const glm::vec2& a1, const glm::vec2& a2, 
		const glm::vec2& b1, const glm::vec2& b2, 
		const glm::vec2& t)
	{
		auto x1 = CosineInterpolate(a1.x, a2.x, t.x);
		auto x2 = CosineInterpolate(b1.x, b2.x, t.x);
		return CosineInterpolate(x1, x2, t.y);
	}

	constexpr auto BilinearTranslate(
		const glm::vec2& a1, const glm::vec2& a2,
		const glm::vec2& b1, const glm::vec2& b2,
		const glm::vec2& t1, const glm::vec2& t2,
		const glm::vec2& k, const glm::vec2& t)
	{
		auto x1 = LinearTranslate(a1.x, a2.x, t1.x, t2.x, t.x);
		auto x2 = LinearTranslate(b1.x, b2.x, t1.x, t2.x, t.x);

		return LinearTranslate(x1, x2, k.x, k.y, t.y);
	}

	template<float(*f)(float)>
	constexpr auto SmoothBilinearTranslate(
		const glm::vec2& a1, const glm::vec2& a2,
		const glm::vec2& b1, const glm::vec2& b2,
		const glm::vec2& t1, const glm::vec2& t2,
		const glm::vec2& m, const glm::vec2& t)
	{
		auto x1 = SmoothLinearTranslate<f>(a1.x, a2.x, t1.x, t2.x, t.x);
		auto x2 = SmoothLinearTranslate<f>(b1.x, b2.x, t1.x, t2.x, t.x);

		return SmoothLinearTranslate<f>(x1, x2, m.x, m.y, t.y);
	}
}
