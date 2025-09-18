#pragma once

#include <glad/glad.h>
#include <array>
#include <memory>

#include "ECS.h"
#include "Renderer.h"
#include "Object.h"

namespace Core
{
	class Engine
	{
	public:
		void OnUpdate();
		void Initialize();
		void OnViewportResize(uint32_t width, uint32_t height);

		GLuint GetRenderTextureID() const { return m_Renderer.GetTextureID(); }
		std::vector<Object>& GetObjects() { return m_Objects; }

		void CreateTestCube();
	private:
		ECS m_ECS;
		Renderer m_Renderer;

		std::vector<Object> m_Objects;
	};
}
