#include "ECS.h"

namespace Core
{
	std::string ECS::CreateEntity()
	{
		std::string entity = Core::GenerateUUID();
		m_Entities.push_back(entity);
		return entity;
	}
}
