#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Log.h"
#include "WindowEvents.h"

namespace Core
{
	class Window
	{
	public:
		Window();

		void Initialize(uint32_t width, uint32_t  height, const char* title);

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		GLFWwindow* GetWindow() const { return m_Window.get(); }

		void AddEventListener(WindowEventListener* listener);
		void RemoveEventListener(WindowEventListener* listener);

	private:
		static void GLFWWindowCloseCallback(GLFWwindow* window);
		static void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);
		static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		uint32_t m_Width, m_Height;

		std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> m_Window;

		std::vector<WindowEventListener*> m_Listeners;
	};
}