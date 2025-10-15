#include "AxisArrow.h"

namespace Core
{
	AxisArrow::AxisArrow(ECS& ecs):
		Object(ecs)
	{
		ASSERT_MSG(Count < 3, "There can only be a maximum of 3 axis arrows in the world.");

		Count++;
		MeshData meshData = Mesh::LoadFromObj(FileUtils::GetObjPath("AxisArrow"));
		AddComponent<Mesh>(meshData);

		SetVisible(false);
	}
}