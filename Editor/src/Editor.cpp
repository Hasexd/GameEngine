#include "Editor.h"

#include <print>

namespace
{
	void ErrorCallback(int error, const char* description)
	{
		std::println("GLFW Error {}: {}", error, description);
	}
}

Editor::Editor():
	m_Window(nullptr, glfwDestroyWindow)
{
	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit())
	{
		std::println("Failed to initialize GLFW");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_Width = 1920;
	m_Height = 1080;

	m_Window.reset(glfwCreateWindow(m_Width, m_Height, "Engine Editor", nullptr, nullptr));

	if (!m_Window)
	{
		std::println("Failed to create a GLFW window");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window.get());
	gladLoadGL();
	glfwSetWindowUserPointer(m_Window.get(), this);
	glfwSwapInterval(1);

	SetWindowCallbacks();
}

void Editor::Run()
{
	while (m_Running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(m_Window.get());
		glfwPollEvents();
	}

	glfwTerminate();
}

void Editor::SetWindowCallbacks()
{
	glfwSetWindowCloseCallback(m_Window.get(), [](GLFWwindow* window)
		{
			Editor* editor = static_cast<Editor*>(glfwGetWindowUserPointer(window));
			if (editor)
				editor->m_Running = false;
		});

	glfwSetFramebufferSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height)
		{
			Editor* editor = static_cast<Editor*>(glfwGetWindowUserPointer(window));

			if (editor)
			{
				editor->m_Width = static_cast<uint32_t>(width);
				editor->m_Height = static_cast<uint32_t>(height);
			}
			glViewport(0, 0, width, height);
		});
}