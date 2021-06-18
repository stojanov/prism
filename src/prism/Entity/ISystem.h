#pragma once

#include <bitset>
#include <phmap.h>

#include "Definitions.h"
#include "ISystem.h"

namespace Prism::Entity
{
	class ISystem
	{
	public:
		virtual ~ISystem() = default;
		virtual void OnUpdate() const = 0;
		virtual void OnSystemEvent() const = 0;
		virtual void OnDraw() const = 0;

		template<typename T, typename ...Args>
		void WatchComponents()
		{
			m_ActiveComponents[GetComponentType<T>()] = true;
		}

		const std::bitset<MAX_COMPONENTS>& ActiveComponents()
		{
			return m_ActiveComponents;
		}
		void InsertEntity(Entity entityId);
		void RemoveEntity(Entity entityId);
	private:
		std::bitset<MAX_COMPONENTS> m_ActiveComponents;
		phmap::flat_hash_set<Entity> m_ActiveEntities;
	};
}