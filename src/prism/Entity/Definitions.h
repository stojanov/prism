#pragma once

namespace Prism::Entity
{
	using ComponentId = uint16_t;
	using Entity = uint32_t;

	constexpr ComponentId MAX_COMPONENTS = 64;
	constexpr Entity MAX_ENTITIES = 5000;

	using EntityCountType = uint32_t;

	namespace detail
	{
		inline ComponentId GenerateComponentId()
		{
			static ComponentId counter = 0;
			return counter++;
		}
	}

	template<typename T>
	inline ComponentId GetComponentType()
	{
		static ComponentId Id = detail::GenerateComponentId();
		return Id;
	}
}