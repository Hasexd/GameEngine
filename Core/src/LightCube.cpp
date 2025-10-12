#include "LightCube.h"


namespace Core
{
	LightCube::LightCube(ECS& ecs):
		Object(ecs)
	{
		MeshData meshData = Mesh::LoadFromObj(FileUtils::GetObjPath("cube"));
		AddComponent<Mesh>(meshData);
	}
}