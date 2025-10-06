#include "Editor.h"

Editor::Editor() :
    m_ViewportWidth(0), m_ViewportHeight(0)
{
    m_Application.Initialize();
    m_Application.GetWindowRef().AddEventListener(this);

    InitializeImGui();

    m_EditorCamera = std::make_shared<Core::Camera>();
    m_Application.SetActiveCamera(m_EditorCamera);

	Core::Input::SetWindow(&m_Application.GetWindowRef());

    m_Running = true;
}

void Editor::InitializeImGui()
{
    GLFWwindow* window = m_Application.GetWindow();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Editor::Run()
{
    while (m_Running)
    {
        float currentFrame = glfwGetTime();
        m_DeltaTime = currentFrame - m_LastFrame;
        m_LastFrame = currentFrame;

        if (m_SelectedObject)
        {
            Core::Transform* transform = m_SelectedObject->GetComponent<Core::Transform>();

            transform->RotationX += 100.0f * m_DeltaTime;
			transform->RotationY += 100.0f * m_DeltaTime;
			transform->RotationZ += 100.0f * m_DeltaTime;
        }

        glfwPollEvents();
        ProcessInput();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderImGui();

        m_Application.Update();
        glfwSwapBuffers(m_Application.GetWindow());
    }
}

void Editor::ProcessInput()
{
    using namespace Core;

    if (m_RightMousePressed)
    {
        if(Input::IsKeyPressed(KeyInput::W))
			m_EditorCamera->ProcessKeyboard(Core::Direction::FORWARD, m_DeltaTime);
		if (Input::IsKeyPressed(KeyInput::S))
			m_EditorCamera->ProcessKeyboard(Core::Direction::BACKWARD, m_DeltaTime);
		if (Input::IsKeyPressed(KeyInput::A))
			m_EditorCamera->ProcessKeyboard(Core::Direction::LEFT, m_DeltaTime);
		if (Input::IsKeyPressed(KeyInput::D))
			m_EditorCamera->ProcessKeyboard(Core::Direction::RIGHT, m_DeltaTime);
    }

	if (Input::IsKeyPressed(KeyInput::ESCAPE))
		m_SelectedObject = nullptr;

	if (Input::IsKeyPressed(KeyInput::CTRL) && Input::IsKeyPressed(KeyInput::R))
	{
		if (!m_ReloadShortcutPressed)
		{
			m_Application.ReloadShaders();
			m_ReloadShortcutPressed = true;
		}
	}
	else
	{
		m_ReloadShortcutPressed = false;
	}
}

void Editor::RenderImGui()
{
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 contentRegionMin = ImGui::GetWindowContentRegionMin();
    m_ViewportPosition = ImVec2(windowPos.x + contentRegionMin.x, windowPos.y + contentRegionMin.y);

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    if (m_ViewportWidth != static_cast<uint32_t>(viewportSize.x) || m_ViewportHeight != static_cast<uint32_t>(viewportSize.y))
    {
        m_ViewportWidth = static_cast<uint32_t>(viewportSize.x);
        m_ViewportHeight = static_cast<uint32_t>(viewportSize.y);
        m_Application.OnViewportResize(m_ViewportWidth, m_ViewportHeight);
    }

    m_IsViewportHovered = ImGui::IsWindowHovered();

    ImGui::Image(static_cast<ImTextureID>(m_Application.GetRenderTextureID()), viewportSize);
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::Begin("Scene Hierarchy");
    for (auto& object : m_Application.GetObjects())
    {
        bool isSelected = (m_SelectedObject.get() == object.get());
        if (ImGui::Selectable(object->GetName().c_str(), isSelected))
        {
            m_SelectedObject = object;
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
    glfwMakeContextCurrent(m_Application.GetWindow());
}

void Editor::OnWindowClose()
{
    m_Running = false;
}

void Editor::OnFramebufferSize(int width, int height)
{
    m_Width = static_cast<uint32_t>(width);
    m_Height = static_cast<uint32_t>(height);
}

void Editor::OnMouseButton(int button, int action, int mods)
{
    using namespace Core;

    if (Input::IsMouseButtonPressed(MouseInput::MOUSE_RIGHT))
    {
        m_RightMousePressed = true;
        Core::Input::SetCursorMode(Core::CursorMode::DISABLED);
        m_FirstMouse = true;
    }
    else if (Input::IsMouseButtonReleased(MouseInput::MOUSE_RIGHT))
    {
        m_RightMousePressed = false;
		Core::Input::SetCursorMode(Core::CursorMode::NORMAL);
    }
    else if (Input::IsMouseButtonPressed(MouseInput::MOUSE_LEFT))
    {
        if (m_IsViewportHovered && !m_RightMousePressed)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(m_Application.GetWindow(), &mouseX, &mouseY);

            float viewportMouseX = mouseX - m_ViewportPosition.x;
            float viewportMouseY = mouseY - m_ViewportPosition.y;

            if (viewportMouseX >= 0 && viewportMouseX < m_ViewportWidth &&
                viewportMouseY >= 0 && viewportMouseY < m_ViewportHeight)
            {
                auto hit = m_Application.ScreenToWorldRaycast(
                    viewportMouseX,
                    viewportMouseY,
                    m_ViewportWidth,
                    m_ViewportHeight
                );

                if (hit.Hit)
                    LOG_INFO("Hit object: {}\n", hit.Object->GetName());
                else
                    LOG_INFO("No object hit.");

                m_SelectedObject = hit.Hit ? hit.Object : nullptr;
            }
        }
    }
}

void Editor::OnCursorPos(double xpos, double ypos)
{
    if (!m_RightMousePressed) 
        return;

    if (m_FirstMouse)
    {
        m_LastX = xpos;
        m_LastY = ypos;
        m_FirstMouse = false;
        return;
    }

    float xoffset = xpos - m_LastX;
    float yoffset = ypos - m_LastY;

    m_LastX = xpos;
    m_LastY = ypos;

    m_EditorCamera->ProcessMouseMovement(xoffset, yoffset);
}

void Editor::OnScroll(double xoffset, double yoffset)
{
    if (m_IsViewportHovered)
    {
        m_EditorCamera->ProcessMouseScroll(yoffset);
    }
}

void Editor::OnKey(int key, int scancode, int action, int mods)
{
    
}

Editor::~Editor()
{
    m_Application.GetWindowRef().RemoveEventListener(this);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}