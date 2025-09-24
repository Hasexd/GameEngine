#pragma once

#include <functional>
#include <vector>

namespace Core
{
	using WindowCloseCallback = std::function<void()>;
	using FramebufferSizeCallback = std::function<void(int width, int height)>;
	using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
	using CursorPosCallback = std::function<void(double xpos, double ypos)>;
	using ScrollCallback = std::function<void(double xoffset, double yoffset)>;
	using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;

	class WindowEventListener
	{
	public:
		virtual ~WindowEventListener() = default;

		virtual void OnWindowClose() {}
		virtual void OnFramebufferSize(int width, int height) {}
		virtual void OnMouseButton(int button, int action, int mods) {}
		virtual void OnCursorPos(double xpos, double ypos) {}
		virtual void OnScroll(double xoffset, double yoffset) {}
		virtual void OnKey(int key, int scancode, int action, int mods) {}
	};
}