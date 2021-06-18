#pragma once

#include <phmap.h>
#include <queue>
#include <bitset>

#include "Definitions.h"
#include "ComponentArray.h"
#include "prism/Core/Pointers.h"
#include "ISystem.h"

namespace Prism::Entity
{
	class Registry
	{
	public:
		Registry();

		Entity CreateEntity();
		void DestroyEntity(Entity entityId);

		template<typename T>
		T& EmplaceComponent(Entity entityId)
		{
			if (m_ComponentArrays.size() >= MAX_COMPONENTS)
			{
				PR_CORE_ERROR("(Registry) Maximum allowed components registered");
				return T();
			}
			if (auto i = m_ActiveEntities.find(entityId); i == m_ActiveEntities.end())
			{
				PR_CORE_ERROR("(Registry) (EmplaceComponent) Cannot find entity with id {0}", entityId);
				return T();
			}
			const auto CompId = GetComponentType<T>();
			if (auto i = m_ComponentArrays.find(CompId); i == m_ComponentArrays.end())
			{
				m_ComponentArrays[CompId] = MakeRef<detail::ComponentArray<T>>();
			}

			m_ActiveEntities[entityId].Components[CompId] = true;

			return GetComponentArray<T>()->EmplaceComponent(entityId);
		}

		template<typename T>
		T& GetComponent(Entity entityId)
		{
			if (auto i = m_ActiveEntities.find(entityId); i == m_ActiveEntities.end())
			{
				PR_CORE_ERROR("(Registry) (GetComponent) Cannot find entity with id {0}", entityId);
				return T();
			}

			if (!m_ActiveEntities[entityId].Components[GetComponentType<T>()])
			{
				PR_CORE_ERROR("(Registry) Component of type {0} not registered with entity {1}", typeid(T).name(), entityId);
				return T();
			}

			return GetComponentArray<T>()->GetComponent(entityId);
		}


		// Temporary, will implement a full system in the future
		void ForEach(std::function<void(Entity)> func);

		// WIP
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<T, ISystem>>>
		void RegisterSystem()
		{

		}

		template<typename T>
		void DetachSystem()
		{

		}

		//void RefreshSystems();
		//void RefreshSystem();
	private:
		// Struct in case i want to add multiple properties
		struct EntityDesc
		{
			std::bitset<MAX_COMPONENTS> Components;
		};
		std::queue<Entity> m_AvalibleEntities;
		phmap::flat_hash_map<Entity, EntityDesc> m_ActiveEntities;
		// hash map in case i want to delete components at runtime without any cost
		phmap::node_hash_map<ComponentId, Ref<detail::IComponentArray>> m_ComponentArrays;
		int m_ActiveEntityCount{ 0 };

		template<typename T>  
		Ref<detail::ComponentArray<T>> GetComponentArray()
		{
			const auto CompId = GetComponentType<T>();
			if (auto i = m_ComponentArrays.find(CompId); i == m_ComponentArrays.end())
			{
				PR_CORE_ERROR("(Registry) There's not a valid component registered, type {0}", typeid(T).name());
				return nullptr;
			}

			return std::static_pointer_cast<detail::ComponentArray<T>>(m_ComponentArrays[CompId]);
		}
	};
}