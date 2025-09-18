#pragma once

#include "Component.h"

#include <glad/glad.h>

namespace Core
{
	struct Mesh : Component
	{

		Mesh(float* vertices, size_t vertexCount, unsigned int* indices, size_t indexCount);
		~Mesh();

		unsigned int VAO, VBO, EBO;
		size_t IndexCount;
	};
}

