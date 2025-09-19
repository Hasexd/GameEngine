#include "Cube.h"

namespace Core
{
	Cube::Cube(ECS& ecs):
		Object(ecs) 
	{
		static float vertices[] = {
			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		static unsigned int indices[] = {
			0, 1, 2,  2, 3, 0,
			4, 5, 6,  6, 7, 4,
			7, 3, 0,  0, 4, 7,
			1, 5, 6,  6, 2, 1,
			3, 2, 6,  6, 7, 3,
			0, 1, 5,  5, 4, 0
		};

		AddComponent<Mesh>(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int));
	}
}