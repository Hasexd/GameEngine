#pragma once

#include <stdint.h>
#include <memory>
#include <array>

#include <glad/glad.h>
#include <mat4x2.hpp>

#include "Object.h"
#include "Shader.h"
#include "Mesh.h"
#include "Log.h"
#include "FileUtils.h"
#include "Camera.h"

namespace Core
{
	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer();

		void Initialize();
		void Render(const std::vector<std::shared_ptr<Object>>& objects);

		void SetViewportSize(uint32_t width, uint32_t height);

		GLuint GetTextureID() const { return m_TextureID; }

		void SetActiveCamera(const std::shared_ptr<Camera>& camera);
		std::shared_ptr<Camera> GetActiveCamera() const { return m_ActiveCamera; }

	private:
		void SetupFramebuffer();
		glm::mat4 GetModelMatrix(const Transform& transform) const;

	private:
		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;

		GLuint m_FramebufferID;
		GLuint m_TextureID;
		GLuint m_DepthBufferID;

		glm::mat4 m_ProjectionMatrix;

		std::shared_ptr<Camera> m_ActiveCamera;

		std::unique_ptr<Shader> m_Shader;

	};
}