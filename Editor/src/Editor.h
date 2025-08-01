#pragma once

#include <memory>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Editor
{
public:
	Editor();
	void Run();
private:
	void SetWindowCallbacks();
private:
	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> m_Window;
	bool m_Running = true;
	uint32_t m_Width;
	uint32_t m_Height;
};