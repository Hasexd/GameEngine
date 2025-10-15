#include "AxisArrow.h"

namespace Core
{
	AxisArrow::AxisArrow(ECS& ecs):
		Object(ecs)
	{
		ASSERT_MSG(Count < 3, "Instantiating an object of the 'AxisArrow' class is not allowed.");

		Count++;
		MeshData meshData = Mesh::LoadFromObj(FileUtils::GetObjPath("AxisArrow"));
		AddComponent<Mesh>(meshData);

		SetVisible(false);
	}
}