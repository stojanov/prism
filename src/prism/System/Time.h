#pragma once

#include <string>
#include <chrono>
#include "Log.h"

namespace Prism::System::Time
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;
	using Clock = std::chrono::high_resolution_clock;
	
	enum Duration
	{
		Seconds,
		Miliseconds,
		Nanoseconds
	};

	template<Duration t, bool fname = false>
	static constexpr char* GetDurationAsString()
	{
		if constexpr (t == Seconds)
		{
			return fname ? "seconds" : "s";
		}
		if constexpr (t == Miliseconds)
		{
			return fname ? "milliseconds" : "ms";
		}
		if constexpr (t == Nanoseconds)
		{
			return fname ? "nanoseconds" : "ns";
		}
	}

	template<Duration duration, typename T>
	static long long DurationCast(T t)
	{
		if constexpr (duration == Seconds)
		{
			return std::chrono::duration_cast<std::chrono::seconds>(t).count();
		}
		if constexpr (duration == Miliseconds)
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
		}
		if constexpr (duration == Nanoseconds)
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(t).count();
		}
	};
}