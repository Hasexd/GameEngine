#pragma once

#include <glad/glad.h>
#include <array>
#include <memory>

#include "Window.h"
#include "ECS.h"
#include "Renderer.h"
#include "Object.h"
#include "Cube.h"
#include "Camera.h"
#include "Physics.h"
#include "WindowEvents.h"

namespace Core
{
	class Application
	{
	public:
		void Update();
		void Initialize();
		void OnViewportResize(uint32_t width, uint32_t height);

		GLuint GetRenderTextureID() const { return m_Renderer.GetTextureID(); }
		std::vector<std::shared_ptr<Object>>& GetObjects() { return m_Objects; }

		void SetActiveCamera(const std::shared_ptr<Camera>& camera) { m_Renderer.SetActiveCamera(camera); };

		RaycastHit Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance = FLT_MAX);
		RaycastHit ScreenToWorldRaycast(float mouseX, float mouseY, float screenWidth, float screenHeight);

		template<typename T>
		requires(std::is_base_of_v<Object, T>)
		void AddObject();

		void RemoveObject(const std::shared_ptr<Object>& object);

		GLFWwindow* GetWindow() const { return m_Window.GetWindow(); }
		Window& GetWindowRef() { return m_Window; }

	private:
		void SyncPhysicsWorld();
	private:
		ECS m_ECS;
		Renderer m_Renderer;
		PhysicsWorld m_PhysicsWorld;
		Window m_Window;

		std::vector<std::shared_ptr<Object>> m_Objects;
	};

	template<typename T>
	requires(std::is_base_of_v<Object, T>)
	void Application::AddObject()
	{
		std::shared_ptr<T> object = std::make_shared<T>(m_ECS);

		m_Objects.emplace_back(object);
		m_PhysicsWorld.RegisterObject(object);
	}
}
