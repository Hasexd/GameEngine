#include "AxisArrow.h"

namespace Core
{
	AxisArrow::AxisArrow(ECS& ecs):
		Object(ecs)
	{
		MeshData meshData = Mesh::LoadFromObj(FileUtils::GetObjPath("AxisArrow"));

		AddComponent<Mesh>(meshData);
	}
}