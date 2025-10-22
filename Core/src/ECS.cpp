#include "ECS.h"

namespace Core
{
	UUID ECS::CreateEntity()
	{
		UUID entity;
		m_Entities.push_back(entity);
		return entity;
	}
}
