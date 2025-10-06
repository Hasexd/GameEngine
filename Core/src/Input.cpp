#include "Input.h"

namespace Core
{
	Window* Input::m_Window = nullptr;

	const std::unordered_map<KeyInput, uint32_t> Input::m_KeyMap = {
		{ KeyInput::W, GLFW_KEY_W },
		{ KeyInput::S, GLFW_KEY_S },
		{ KeyInput::A, GLFW_KEY_A },
		{ KeyInput::D, GLFW_KEY_D },
		{ KeyInput::R , GLFW_KEY_R},
		{ KeyInput::ESCAPE, GLFW_KEY_ESCAPE },
		{ KeyInput::CTRL, GLFW_KEY_LEFT_CONTROL }
	};

	const std::unordered_map<MouseInput, uint32_t> Input::m_MouseButtonMap = {
		{ MouseInput::MOUSE_LEFT, GLFW_MOUSE_BUTTON_LEFT },
		{ MouseInput::MOUSE_RIGHT, GLFW_MOUSE_BUTTON_RIGHT }
	};

	const std::unordered_map<CursorMode, uint32_t> Input::m_CursorModeMap = {
		{ CursorMode::NORMAL, GLFW_CURSOR_NORMAL },
		{ CursorMode::DISABLED, GLFW_CURSOR_DISABLED },
		{ CursorMode::HIDDEN, GLFW_CURSOR_HIDDEN }
	};

	bool Input::IsKeyPressed(KeyInput key)
	{
		auto it = m_KeyMap.find(key);
		if (it != m_KeyMap.end())
		{
			int state = glfwGetKey(m_Window->GetWindow(), it->second);
			return state == GLFW_PRESS;
		}
		return false;
	}

	bool Input::IsKeyReleased(KeyInput key)
	{
		auto it = m_KeyMap.find(key);
		if (it != m_KeyMap.end())
		{
			int state = glfwGetKey(m_Window->GetWindow(), it->second);
			return state == GLFW_RELEASE;
		}
		return false;
	}

	bool Input::IsMouseButtonPressed(MouseInput button)
	{
		auto it = m_MouseButtonMap.find(button);
		if (it != m_MouseButtonMap.end())
		{
			int state = glfwGetMouseButton(m_Window->GetWindow(), it->second);
			return state == GLFW_PRESS;
		}
		return false;
	}

	bool Input::IsMouseButtonReleased(MouseInput button)
	{
		auto it = m_MouseButtonMap.find(button);
		if (it != m_MouseButtonMap.end())
		{
			int state = glfwGetMouseButton(m_Window->GetWindow(), it->second);
			return state == GLFW_RELEASE;
		}
		return false;
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		auto it = m_CursorModeMap.find(mode);

		if(it != m_CursorModeMap.end())
		{
			glfwSetInputMode(m_Window->GetWindow(), GLFW_CURSOR, it->second);
		}
	}
}
