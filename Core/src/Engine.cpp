#include "Engine.h"

namespace Core
{
	void Engine::OnUpdate()
	{
		if (!m_Objects.empty())
		{
			Transform* transform = m_Objects[0].GetComponent<Transform>();
			if (transform)
			{
				transform->RotationX += 1.0f;
			}
		}

		m_Renderer.Render(m_Objects);
	}

	void Engine::Initialize()
	{
		m_Renderer.Initialize();
		CreateTestCube();
	}

	void Engine::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_Renderer.SetViewportSize(width, height);
	}

	void Engine::CreateTestCube()
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

		Object cube(m_ECS);

		cube.AddComponent<Mesh>(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int));

		m_Objects.push_back(cube);
	}
}

