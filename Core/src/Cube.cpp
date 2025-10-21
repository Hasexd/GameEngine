#include "Cube.h"

namespace Core
{
    Cube::Cube(ECS& ecs) :
        Object(ecs)
    {
        AddComponent<Mesh>(Mesh::CreateFromObj(FileUtils::GetObjPath("Cube")));
    }
}