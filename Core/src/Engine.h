#pragma once

#include <glad/glad.h>
#include <array>
#include <memory>

#include "ECS.h"
#include "Renderer.h"
#include "Object.h"
#include "Cube.h"

namespace Core
{
	class Engine
	{
	public:
		void OnUpdate(const std::shared_ptr<Object>& selectedObject = nullptr);
		void Initialize();
		void OnViewportResize(uint32_t width, uint32_t height);

		GLuint GetRenderTextureID() const { return m_Renderer.GetTextureID(); }
		std::vector<std::shared_ptr<Object>>& GetObjects() { return m_Objects; }

	private:
		ECS m_ECS;
		Renderer m_Renderer;

		std::vector<std::shared_ptr<Object>> m_Objects;
	};
}
