#include "Renderer.h"

#include "Log.h"

namespace Core
{
	void Renderer::Initialize()
	{
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Renderer::Render()
	{
		if (m_TextureData)
		{
			uint32_t r = (uint32_t)(m_TextureColorFloat[2] * 255.0f);
			uint32_t g = (uint32_t)(m_TextureColorFloat[1] * 255.0f);
			uint32_t b = (uint32_t)(m_TextureColorFloat[0] * 255.0f);
			uint32_t colorWithAlpha = (0xFF << 24) | (r << 16) | (g << 8) | b;

			std::fill(m_TextureData.get(), m_TextureData.get() + (m_ViewportWidth * m_ViewportHeight), colorWithAlpha);

			glBindTexture(GL_TEXTURE_2D, m_TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_ViewportWidth, m_ViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureData.get());
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			LOG_INFO("No texture data available to render");
		}
	}

	void Renderer::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_TextureData.reset(new uint32_t[m_ViewportWidth * m_ViewportHeight]);

		glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);
	}
}