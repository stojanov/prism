#pragma once
#include <memory>

namespace Prism
{
	template<typename T>
	using Ptr = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ...Args>
	constexpr Ptr<T> MakePtr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	constexpr Ref<T> MakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename To, typename From>
	std::unique_ptr<To> DynamicPtrCast(std::unique_ptr<From>&& p)
	{
		// Must have a virtual destructor
		if (To* cast = dynamic_cast<To*>(p.get()))
		{
			std::unique_ptr<To> result(cast, std::move(p.get_deleter()));
			p.release();
			return result;
		}
		return std::unique_ptr<To>(nullptr);
	}
}