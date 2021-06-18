
#include <prism/System/Debug.h>
#include "ISystem.h"

namespace Prism::Entity
{
	void ISystem::InsertEntity(Entity entityId)
	{
		if (m_ActiveEntities.find(entityId) == m_ActiveEntities.end())
		{
			PR_CORE_ERROR("(Entity::System) (InsertEntity) Entity already added.");
			return;
		}
		m_ActiveEntities.insert(entityId);
	}

	void ISystem::RemoveEntity(Entity entityId)
	{
		if (m_ActiveEntities.find(entityId) != m_ActiveEntities.end())
		{
			PR_CORE_ERROR("(Entity::System) (RemoveEntity) Entity doesn't exist.");
			return;
		}
		m_ActiveEntities.erase(entityId);
	}
}