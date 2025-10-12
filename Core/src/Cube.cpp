#include "Cube.h"

namespace Core
{
    Cube::Cube(ECS& ecs) :
        Object(ecs)
    {
        MeshData meshData = Mesh::LoadFromObj(FileUtils::GetObjPath("cube"));

        AddComponent<Mesh>(meshData);
    }
}