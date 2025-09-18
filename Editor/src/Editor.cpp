#include "Editor.h"
#include "Transform.h"
#include "Log.h"

#include <print>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace
{
	void ErrorCallback(int error, const char* description)
	{
		LOG_ERROR("GLFW Error ({}): {}", error, description);
	}
}

Editor::Editor():
	m_Window(nullptr, glfwDestroyWindow), m_Running(false), m_Width(1920), m_Height(1080), m_ViewportWidth(0), m_ViewportHeight(0)
{
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

	m_Window.reset(glfwCreateWindow(m_Width, m_Height, "Engine Editor", nullptr, nullptr));

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

	SetWindowCallbacks();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_Window.get(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	m_Engine.Initialize();

	m_Running = true;
}

void Editor::Run()
{
	while (m_Running)
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		RenderImGui();
		m_Engine.OnUpdate();

		glfwSwapBuffers(m_Window.get());
	}
}

void Editor::RenderImGui()
{
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	if(m_ViewportWidth != static_cast<uint32_t>(viewportSize.x) || m_ViewportHeight != static_cast<uint32_t>(viewportSize.y))
	{
		m_ViewportWidth = static_cast<uint32_t>(viewportSize.x);
		m_ViewportHeight = static_cast<uint32_t>(viewportSize.y);
		m_Engine.OnViewportResize(m_ViewportWidth, m_ViewportHeight);
	}

	ImGui::Image(static_cast<ImTextureID>(m_Engine.GetRenderTextureID()), viewportSize);
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("Scene Hierarchy");
	ImGui::End();

	ImGui::Begin("Inspector");
	ImGui::End();

	ImGui::Begin("Assets");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(m_Window.get());
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

Editor::~Editor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	if(m_Window)
		m_Window.reset();

	glfwTerminate();
}