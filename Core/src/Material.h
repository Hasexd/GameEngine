#pragma once

#include <glm.hpp>

namespace Core
{
    struct Material
    {
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        float Shininess;
    };
}



