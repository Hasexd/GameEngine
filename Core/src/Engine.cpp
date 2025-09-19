#include "Engine.h"

namespace Core
{
	void Engine::OnUpdate()
	{

		Transform* transform = m_Objects[0]->GetComponent<Transform>();

		transform->RotationX += 1.0f;
		transform->RotationY += 1.0f;
		transform->RotationZ += 1.0f;

		m_Renderer.Render(m_Objects);
	}

	void Engine::Initialize()
	{
		m_Renderer.Initialize();

		m_Objects.emplace_back(std::make_shared<Cube>(m_ECS));
	}

	void Engine::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_Renderer.SetViewportSize(width, height);
	}
}

