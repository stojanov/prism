#pragma once
#include <type_traits>

namespace Prism::Voxel
{
	template<typename T>
	using EnableIfNumber = std::enable_if_t<std::conjunction_v<std::is_integral<T>>, std::is_floating_point<T>>;

	template<typename T, typename = EnableIfNumber<T>>
	struct Vec2
	{
		T x;
		union
		{
			T y;
			T z;
		};
		
		Vec2(T _x, T _y)
			:
			x(_x),
			y(_y)
		{}

		Vec2(const Vec2<T>& vec)
			:
			x(vec.x),
			y(vec.y)
		{}

		Vec2(int v)
			:
			x(v),
			y(v)
		{}

		const Vec2<T>& operator+(const Vec2<T>& other)
		{
			return { x + other.x, y + other.y};
		}

		const Vec2<T>& operator-(const Vec2<T>& other)
		{
			return { x + other.x, y + other.y };
		}

		const Vec2<T>& operator/(const Vec2<T>& other)
		{
			return { x / other.x, y / other.y };
		}

		const Vec2<T>& operator*(const Vec2<T>& other)
		{
			return { x * other.x, y * other.y };
		}
		
		Vec2<T>& operator+=(const Vec2<T>& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		Vec2<T>& operator-=(const Vec2<T>& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		Vec2<T>& operator*=(const Vec2<T>& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}

		Vec2<T>& operator/=(const Vec2<T>& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		bool operator==(const Vec2<T>& other) const noexcept
		{
			return (x == other.x) && (y == other.y);
		}

		size_t operator()(const Vec2<T>& vec) const noexcept
		{
			std::hash<T> hasher;
			auto a = hasher(vec.x);
			auto b = hasher(vec.y);
			// (a ^ b) >> 2
			return b + 0x9e3779b9 + (a << 6) + (a >> 2);
		}
	};

	template<typename T, typename = EnableIfNumber<T>>
	T Vec2Dot(const Vec2<T>& a, const Vec2<T>& b)
	{
		return { a.x * b.x + a.y * b.y };
	}

	template<typename T, typename U, typename = EnableIfNumber<T>>
	const Vec2<U>& Vec2Cross(const Vec2<T>& a, const Vec2<T>& b)
	{

	}

	using Vec2i = Vec2<int>;
}
