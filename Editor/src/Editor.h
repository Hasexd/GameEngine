#pragma once

#include <memory>

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
private:
	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> m_Window;
	bool m_Running = true;

	uint32_t m_Width, m_Height;
	uint32_t m_ViewportWidth, m_ViewportHeight;

	Core::Engine m_Engine;
};