#include "Cube.h"

namespace Core
{
    Cube::Cube(ECS& ecs) :
        Object(ecs)
    {
        MeshData meshData = Mesh::LoadFromObj(FileUtils::GetObjPath("Cube"));

        AddComponent<Mesh>(meshData);
    }
}