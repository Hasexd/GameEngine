#pragma once

#include <stdint.h>
#include <memory>

#include <glad/glad.h>

namespace Core
{
	class Renderer
	{
	public:
		Renderer() = default;

		void Initialize();
		void Render();

		void SetViewportSize(uint32_t width, uint32_t height);
		float* GetTextureColorArray() { return m_TextureColorFloat; }

		GLuint GetTextureID() const { return m_TextureID; }
	private:
		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;

		float m_TextureColorFloat[3] = {0.0f};

		std::unique_ptr<uint32_t[]> m_TextureData;

		GLuint m_TextureID;
	};
}