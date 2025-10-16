#pragma once

#include <glad/glad.h>
#include <array>
#include <memory>
#include <future>
#include <string>

#include "Window.h"
#include "ECS.h"
#include "Renderer.h"
#include "Object.h"
#include "Cube.h"
#include "Camera.h"
#include "Physics.h"
#include "WindowEvents.h"
#include "FileWatcher.h"
#include "Gizmo.h"

namespace Core
{
	class Application
	{
	public:
		~Application();

		void Update(const std::shared_ptr<Object>& selectedObject = nullptr);
		void Initialize();
		void OnViewportResize(uint32_t width, uint32_t height);

		GLuint GetRenderTextureID() const { return m_Renderer.GetTextureID(); }
		std::vector<std::shared_ptr<Object>>& GetObjects() { return m_Objects; }

		void SetActiveCamera(const std::shared_ptr<Camera>& camera) { m_Renderer.SetActiveCamera(camera); };

		RaycastHit Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance = FLT_MAX);
		RaycastHit ScreenToWorldRaycast(float mouseX, float mouseY, float screenWidth, float screenHeight);

		template<typename T>
		requires(std::is_base_of_v<Object, T>)
		std::shared_ptr<T> AddObject();

		template<typename T>
		requires(std::is_base_of_v<Object, T>)
		std::shared_ptr<T> AddObject(const std::string& name);

		void RemoveObject(const std::shared_ptr<Object>& object);

		template<typename T>
		requires(std::is_base_of_v<Object, T>)
		std::vector<std::shared_ptr<T>> GetAllObjectsOfType();

		GLFWwindow* GetWindow() const { return m_Window.GetWindow(); }
		Window& GetWindowRef() { return m_Window; }

		void ReloadShaders();
		void ShowGizmos();
		void HideGizmos();

	private:
		void SyncPhysicsWorld();
		void MonitorShaderChanges();
	private:
		ECS m_ECS;
		Renderer m_Renderer;
		PhysicsWorld m_PhysicsWorld;
		Window m_Window;

		FileWatcher m_FileWatcher;
		std::future<void> m_FileWatcherFuture;

		std::vector<std::shared_ptr<Object>> m_Objects;
		std::vector<Gizmo> m_Gizmos;

		std::vector<std::string> m_ModifiedShaderFiles;
		std::atomic<bool> m_ShadersNeedReload = false;
	};

	template<typename T>
	requires(std::is_base_of_v<Object, T>)
	std::shared_ptr<T> Application::AddObject()
	{
		std::shared_ptr<T> object = std::make_shared<T>(m_ECS);

		m_Objects.push_back(object);
		m_PhysicsWorld.RegisterObject(object);

		return object;
	}

	template<typename T>
	requires(std::is_base_of_v<Object, T>)
	std::shared_ptr<T> Application::AddObject(const std::string& name)
	{
		std::shared_ptr<T> object = std::make_shared<T>(m_ECS);
		object->SetName(name);

		m_Objects.push_back(object);
		m_PhysicsWorld.RegisterObject(object);

		return object;
	}

	template<typename T>
	requires(std::is_base_of_v<Object, T>)
	std::vector<std::shared_ptr<T>> Application::GetAllObjectsOfType()
	{
		std::vector<std::shared_ptr<T>> results;
		for (const auto& obj : m_Objects)
		{
			if (std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(obj))
			{
				results.push_back(ptr);
			}
		}
		return results;
	}
}
