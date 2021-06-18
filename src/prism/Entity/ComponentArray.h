#include <phmap.h>
#include <array>
#include <functional>

#include "Definitions.h"
#include "prism/System/Log.h"

namespace Prism::Entity::detail
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void DestroyEntity(Entity id) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		T& EmplaceComponent(Entity entityId)
		{
			if (auto i = m_EntToIdx.find(entityId); i != m_EntToIdx.end())
			{
				PR_CORE_ERROR("(ComponentArray) Cannot add component of type {0} to entity of id {1}, only unique components allowed",
					typeid(T).name(), entityId);
				return T();
			}

			m_EntToIdx[entityId] = m_Size;
			m_IdxToEnt[m_Size] = entityId;
			m_Components[m_Size] = T();

			m_Size++;

			return m_Components[m_Size - 1];
		}

		bool DestroyComponent(Entity entityId)
		{
			PR_ASSERT(m_Size > 0, "(ComponentArray) Empty Array, no components to destroy");
			if (auto i = m_EntToIdx.find(entityId); i == m_EntToIdx.end())
			{
				PR_CORE_ERROR("(ComponentArray) Cannot find entity with id {0}", entityId);
				return false;
			}

			if (m_Size == 1)
			{
				m_Size--;
				m_EntToIdx.clear();
				m_IdxToEnt.clear();
				return true;
			}

			auto LastElementIdx = m_Size - 1;
			auto RemovedIdx = m_EntToIdx[entityId];

			m_Components[RemovedIdx] = m_Components[LastElementIdx];

			Entity LastEnt = m_IdxToEnt[LastElementIdx];
			m_EntToIdx[LastEnt] = RemovedIdx;
			m_IdxToEnt[RemovedIdx] = LastEnt;

			m_EntToIdx.erase(LastEnt);
			m_IdxToEnt.erase(LastElementIdx);

			m_Size--;

			return true;
		}

		T& GetComponent(Entity entityId)
		{
			PR_ASSERT(m_Size > 0, "(ComponentArray) Empty Array, cannot find component");
			if (auto i = m_EntToIdx.find(entityId); i == m_EntToIdx.end())
			{
				PR_CORE_ERROR("(ComponentArray) Cannot find entity with id {0}", entityId);
			}

			return m_Components[m_EntToIdx[entityId]];
		}

		void DestroyEntity(Entity entityId)
		{
			DestroyComponent(entityId);
		}

		void ForEach(std::function<void(T&)> func)
		{
			for (int i = 0; i < m_Size; i++)
			{
				func(m_Components[i]);
			}
		}
	private:
		std::array<T, MAX_ENTITIES> m_Components; // Contiguous packed memory access
		phmap::flat_hash_map<Entity, EntityCountType> m_EntToIdx;
		phmap::flat_hash_map<EntityCountType, Entity> m_IdxToEnt;
		uint32_t m_Size{ 0 };
	};
}