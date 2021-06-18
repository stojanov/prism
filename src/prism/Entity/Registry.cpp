#include "Registry.h"

#include "prism/System/Debug.h"


namespace Prism::Entity
{
	Registry::Registry()
	{
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			m_AvalibleEntities.push(i);
		}
	}

	Entity Registry::CreateEntity()
	{
		PR_ASSERT(m_ActiveEntityCount < MAX_ENTITIES, "Max entity count reached.");

		auto id = m_AvalibleEntities.front();
		m_AvalibleEntities.pop();
		m_ActiveEntityCount++;
		m_ActiveEntities[id] = EntityDesc{};
		return id;
	}

	void Registry::DestroyEntity(Entity entityId)
	{
		if (auto i = m_ActiveEntities.find(entityId); i != m_ActiveEntities.end())
		{
			PR_CORE_ERROR("(Registry) (DestroyEntity) Cannot find entity with id {0}", entityId);
			return;
		}

		auto& entity = m_ActiveEntities[entityId];
		for (ComponentId i = 0; i < MAX_COMPONENTS; i++)
		{
			if (entity.Components[i])
			{
				m_ComponentArrays[i]->DestroyEntity(entityId);
			}
		}
		
		m_ActiveEntities.erase(entityId);
		m_AvalibleEntities.push(entityId);
		m_ActiveEntityCount--;
	}


	void Registry::ForEach(std::function<void(Entity)> func)
	{
		for (auto i : m_ActiveEntities)
		{
			func(i.first);
		}
	}
}