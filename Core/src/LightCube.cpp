#include "LightCube.h"


namespace Core
{
	LightCube::LightCube(ECS& ecs):
		Object(ecs)
	{
		AddComponent<Mesh>(Mesh::LoadFromObj(FileUtils::GetObjPath("Cube")));
	}
}