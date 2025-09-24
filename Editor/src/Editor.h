#pragma once

#include <memory>
#include <print>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Application.h"
#include "WindowEvents.h"
#include "Transform.h"
#include "Log.h"
#include "Input.h"


class Editor : public Core::WindowEventListener
{
public:
	Editor();
	~Editor();
	void Run();

	void OnWindowClose() override;
	void OnFramebufferSize(int width, int height) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnCursorPos(double xpos, double ypos) override;
	void OnScroll(double xoffset, double yoffset) override;
	void OnKey(int key, int scancode, int action, int mods) override;

private:
	void InitializeImGui();
	void RenderImGui();
	void ProcessInput();

private:
	uint32_t m_ViewportWidth, m_ViewportHeight;
	uint32_t m_Width, m_Height;

	std::shared_ptr<Core::Camera> m_EditorCamera;

	double m_LastX = 0.0;
	double m_LastY = 0.0;

	ImVec2 m_ViewportPosition = ImVec2(0, 0);

	bool m_FirstMouse = true;
	bool m_RightMousePressed = false;
	bool m_IsViewportHovered = false;

	Core::Application m_Application;
	std::shared_ptr<Core::Object> m_SelectedObject = nullptr;

	double m_DeltaTime = 0.0;
	double m_LastFrame = 0.0;

	bool m_Running = false;
};