#include "Window.h"

namespace
{
	void ErrorCallback(int error, const char* description)
	{
		LOG_ERROR("GLFW Error ({}): {}", error, description);
	}
}

namespace Core
{
	Window::Window() :
		m_Width(0), m_Height(0), m_Window(nullptr, glfwDestroyWindow) {}

	void Window::Initialize(uint32_t width, uint32_t  height, const char* title)
	{
		m_Width = width;
		m_Height = height;

		glfwSetErrorCallback(ErrorCallback);

		if (!glfwInit())
		{
			LOG_CRITICAL("Failed to initialize GLFW");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		m_Window.reset(glfwCreateWindow(m_Width, m_Height, title, nullptr, nullptr));

		if (!m_Window)
		{
			LOG_CRITICAL("Failed to create a GLFW window");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_Window.get());

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			LOG_CRITICAL("Failed to initialize GLAD");
			m_Window.reset();
			glfwTerminate();
			return;
		}

		glfwSetWindowUserPointer(m_Window.get(), this);
		glfwSwapInterval(1);

		glfwSetWindowCloseCallback(m_Window.get(), GLFWWindowCloseCallback);
		glfwSetFramebufferSizeCallback(m_Window.get(), GLFWFramebufferSizeCallback);
		glfwSetMouseButtonCallback(m_Window.get(), GLFWMouseButtonCallback);
		glfwSetCursorPosCallback(m_Window.get(), GLFWCursorPosCallback);
		glfwSetScrollCallback(m_Window.get(), GLFWScrollCallback);
		glfwSetKeyCallback(m_Window.get(), GLFWKeyCallback);
	}

	void Window::AddEventListener(WindowEventListener* listener)
	{
		if (!listener)
			return;

		if (std::find(m_Listeners.begin(), m_Listeners.end(), listener) == m_Listeners.end())
			m_Listeners.push_back(listener);
	}

	void Window::RemoveEventListener(WindowEventListener* listener)
	{
		auto it = std::find(m_Listeners.begin(), m_Listeners.end(), listener);

		if (it != m_Listeners.end())
			m_Listeners.erase(it);
	}

	void Window::GLFWWindowCloseCallback(GLFWwindow* window)
	{
		auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (!win)
			return;

		for (auto* l : win->m_Listeners)
			l->OnWindowClose();
	}

	void Window::GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (!win) 
			return;

		win->m_Width = static_cast<uint32_t>(width);
		win->m_Height = static_cast<uint32_t>(height);

		for (auto* l : win->m_Listeners)
			l->OnFramebufferSize(width, height);
	}

	void Window::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (!win)
			return;

		for (auto* l : win->m_Listeners)
			l->OnMouseButton(button, action, mods);
	}

	void Window::GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (!win)
			return;

		for (auto* l : win->m_Listeners) 
			l->OnCursorPos(xpos, ypos);
	}

	void Window::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (!win)
			return;

		for (auto* l : win->m_Listeners)
			l->OnScroll(xoffset, yoffset);
	}

	void Window::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (!win)
			return;

		for (auto* l : win->m_Listeners)
			l->OnKey(key, scancode, action, mods);
	}
}
