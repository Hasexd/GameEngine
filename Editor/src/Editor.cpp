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


Editor::Editor() :
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

	m_EditorCamera = std::make_shared<Core::Camera>();
	m_Engine.SetActiveCamera(m_EditorCamera);

	m_Running = true;
}

void Editor::Run()
{
	while (m_Running)
	{
		float currentFrame = glfwGetTime();
		m_DeltaTime = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;

		glfwPollEvents();
		ProcessInput();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		RenderImGui();
		m_Engine.OnUpdate(m_SelectedObject);

		glfwSwapBuffers(m_Window.get());
	}
}

void Editor::ProcessInput()
{
	if (m_RightMousePressed && m_IsViewportHovered)
	{
		if (glfwGetKey(m_Window.get(), GLFW_KEY_W) == GLFW_PRESS)
			m_EditorCamera->ProcessKeyboard(Core::Direction::FORWARD, m_DeltaTime);
		if (glfwGetKey(m_Window.get(), GLFW_KEY_S) == GLFW_PRESS)
			m_EditorCamera->ProcessKeyboard(Core::Direction::BACKWARD, m_DeltaTime);
		if (glfwGetKey(m_Window.get(), GLFW_KEY_A) == GLFW_PRESS)
			m_EditorCamera->ProcessKeyboard(Core::Direction::LEFT, m_DeltaTime);
		if (glfwGetKey(m_Window.get(), GLFW_KEY_D) == GLFW_PRESS)
			m_EditorCamera->ProcessKeyboard(Core::Direction::RIGHT, m_DeltaTime);
		if (glfwGetKey(m_Window.get(), GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(m_Window.get(), GLFW_KEY_SPACE) == GLFW_PRESS)
			m_EditorCamera->ProcessKeyboard(Core::Direction::UP, m_DeltaTime);
		if (glfwGetKey(m_Window.get(), GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(m_Window.get(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			m_EditorCamera->ProcessKeyboard(Core::Direction::DOWN, m_DeltaTime);
	}
}

void Editor::RenderImGui()
{
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	if (m_ViewportWidth != static_cast<uint32_t>(viewportSize.x) || m_ViewportHeight != static_cast<uint32_t>(viewportSize.y))
	{
		m_ViewportWidth = static_cast<uint32_t>(viewportSize.x);
		m_ViewportHeight = static_cast<uint32_t>(viewportSize.y);
		m_Engine.OnViewportResize(m_ViewportWidth, m_ViewportHeight);
	}

	m_IsViewportHovered = ImGui::IsWindowHovered();

	ImGui::Image(static_cast<ImTextureID>(m_Engine.GetRenderTextureID()), viewportSize);
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("Scene Hierarchy");

	static bool isRenaming = false;
	static Core::Object* renamingObject = nullptr;
	static char renameBuffer[256] = "";

	for (auto& object : m_Engine.GetObjects())
	{
		if (isRenaming && renamingObject == object.get())
		{
			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText("##Rename", renameBuffer, sizeof(renameBuffer),
				ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				std::string renamed = std::string(renameBuffer);

				if (renamed == "")
				{
					renamed = object->GetName();
				}

				object->SetName(renamed);
				isRenaming = false;
				renamingObject = nullptr;
			}

			if (ImGui::IsKeyPressed(ImGuiKey_Escape))
			{
				isRenaming = false;
				renamingObject = nullptr;
			}
		}
		else
		{
			bool isSelected = (m_SelectedObject.get() == object.get());
			if (ImGui::Selectable(object->GetName().c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					isRenaming = true;
					renamingObject = object.get();
					strcpy(renameBuffer, object->GetName().c_str());
				}
				else
				{
					m_SelectedObject = object;
				}
			}
		}
	}

	ImGui::End();

	ImGui::Begin("Inspector");

	if (m_SelectedObject)
	{
		Core::Transform* transform = m_SelectedObject->GetComponent<Core::Transform>();

		ImGui::Text("Selected object: %s", m_SelectedObject->GetName().c_str());
		ImGui::InputFloat3("Position", &transform->X);
		ImGui::InputFloat3("Rotation", &transform->RotationX);
		ImGui::InputFloat3("Scale", &transform->ScaleX);
	}

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

	glfwSetMouseButtonCallback(m_Window.get(), [](GLFWwindow* window, int button, int action, int mods)
		{
			Editor* editor = static_cast<Editor*>(glfwGetWindowUserPointer(window));
			if (!editor) return;

			if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				if (action == GLFW_PRESS)
				{
					editor->m_RightMousePressed = true;
					if (editor->m_IsViewportHovered)
					{
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						editor->m_FirstMouse = true;
					}
				}
				else if (action == GLFW_RELEASE)
				{
					editor->m_RightMousePressed = false;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
			}
		});

	glfwSetCursorPosCallback(m_Window.get(), [](GLFWwindow* window, double xpos, double ypos)
		{
			Editor* editor = static_cast<Editor*>(glfwGetWindowUserPointer(window));

			if (!editor)
				return;

			if (editor->m_RightMousePressed && editor->m_IsViewportHovered)
			{
				if (editor->m_FirstMouse)
				{
					editor->m_LastX = xpos;
					editor->m_LastY = ypos;
					editor->m_FirstMouse = false;
				}

				float xoffset = xpos - editor->m_LastX;
				float yoffset = ypos - editor->m_LastY;

				editor->m_LastX = xpos;
				editor->m_LastY = ypos;

				editor->m_EditorCamera->ProcessMouseMovement(xoffset, yoffset);
			}
		});

	glfwSetScrollCallback(m_Window.get(), [](GLFWwindow* window, double xoffset, double yoffset)
		{
			Editor* editor = static_cast<Editor*>(glfwGetWindowUserPointer(window));
			if (!editor) return;

			if (editor->m_IsViewportHovered)
			{
				editor->m_EditorCamera->ProcessMouseScroll(yoffset);
			}
		});

	glfwSetKeyCallback(m_Window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				Editor* editor = static_cast<Editor*>(glfwGetWindowUserPointer(window));
				if (editor)
					editor->m_SelectedObject = nullptr;
			}
		});
}

Editor::~Editor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	if (m_Window)
		m_Window.reset();

	glfwTerminate();
}