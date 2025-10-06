#pragma once

#include <unordered_map>

#include "Window.h"

namespace Core
{
	enum class KeyInput
	{
		W = 0,
		S,
		A,
		D,
		R,
		ESCAPE,
		CTRL
	};

	enum class MouseInput
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
	};

	enum class CursorMode
	{
		NORMAL = 0,
		DISABLED,
		HIDDEN
	};

	class Input
	{
	public:
		Input() = delete;

		static bool IsKeyPressed(KeyInput key);
		static bool IsKeyReleased(KeyInput key);
		static bool IsMouseButtonPressed(MouseInput button);
		static bool IsMouseButtonReleased(MouseInput button);	
		static void SetCursorMode(CursorMode mode);
		static void SetWindow(Window* window) { m_Window = window; }

	private:
		static const std::unordered_map<KeyInput, uint32_t> m_KeyMap;
		static const std::unordered_map<MouseInput, uint32_t> m_MouseButtonMap;
		static const std::unordered_map<CursorMode, uint32_t> m_CursorModeMap;
		static Window* m_Window;
	};
}