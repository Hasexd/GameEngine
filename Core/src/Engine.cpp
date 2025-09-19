#include "Engine.h"

namespace Core
{
	void Engine::OnUpdate(const std::shared_ptr<Object>& selectedObject)
	{

		Transform* transform = m_Objects[0]->GetComponent<Transform>();

		transform->RotationX += 1.0f;
		transform->RotationY += 1.0f;
		transform->RotationZ += 1.0f;

		m_Renderer.Render(m_Objects, selectedObject);
	}

	void Engine::Initialize()
	{
		m_Renderer.Initialize();

		m_Objects.emplace_back(std::make_shared<Cube>(m_ECS));
		m_Objects.emplace_back(std::make_shared<Cube>(m_ECS));

		m_Objects[0]->SetName("Cube 1");
		Transform* cubeTransform = m_Objects[0]->GetComponent<Transform>();
		cubeTransform->Z = -5.0f;
		cubeTransform->X = -2.0f;

		m_Objects[1]->SetName("Cube 2");
		cubeTransform = m_Objects[1]->GetComponent<Transform>();
		cubeTransform->Z = -5.0f;
		cubeTransform->X = 2.0f;

	}

	void Engine::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_Renderer.SetViewportSize(width, height);
	}
}

