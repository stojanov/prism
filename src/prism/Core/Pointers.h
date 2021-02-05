#pragma once
#include <memory>

#include "prism/System/Debug.h"

namespace Prism
{
	template<typename T>
	using Ptr = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

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
	Ptr<To> DynamicPtrCast(Ptr<From>&& p)
	{
		// Static assert would also do fine but it won't get the type info 
		PR_CONST_ASSERT(std::has_virtual_destructor_v<To>,
			"(" + static_cast<std::string>(typeid(To).name()) + ") must have a virtual destructor."
		);
		
		if (To* cast = dynamic_cast<To*>(p.get()))
		{
			// Virtual destructor should be called when release
			//std::unique_ptr<To> result(cast, std::move(p.get_deleter()));
			Ptr<To> result(cast);
			p.release();
			return result;
		}

		std::string ToType = typeid(To).name();
		std::string FromType = typeid(From).name();
		
		PR_ASSERT(0, "Cannot do dynamic cast on: " + FromType + " -> " + ToType);
		return Ptr<To>(nullptr);
	}
}
