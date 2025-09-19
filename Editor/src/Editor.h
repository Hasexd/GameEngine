#pragma once

#include <memory>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine.h"


class Editor
{
public:
	Editor();
	~Editor();
	void Run();
private:
	void SetWindowCallbacks();
	void RenderImGui();
	void ProcessInput();

	glm::vec3 ScreenToWorldRay(float screenX, float screenY);
	std::shared_ptr<Core::Object> GetObjectAtRay(const glm::vec3& origin, const glm::vec3& direction);
private:
	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> m_Window;
	bool m_Running = true;

	uint32_t m_Width, m_Height;
	uint32_t m_ViewportWidth, m_ViewportHeight;

	std::shared_ptr<Core::Object> m_SelectedObject = nullptr;
	std::shared_ptr<Core::Camera> m_EditorCamera;

	float m_LastX = 0.0f;
	float m_LastY = 0.0f;
	bool m_FirstMouse = true;
	bool m_RightMousePressed = false;
	bool m_IsViewportHovered = false;

	float m_DeltaTime = 0.0f;
	float m_LastFrame = 0.0f;

	Core::Engine m_Engine;
};