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
#include "LightCube.h"
#include "Gizmo.h"

namespace Core
{
	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer();

		void Initialize();
		void Render(const std::vector<std::shared_ptr<Object>>& objects, const std::shared_ptr<Object>& selectedObject = nullptr);
		void RenderGizmos(const std::vector<Gizmo>& gizmos, const std::shared_ptr<Object>& selectedObject);

		void SetViewportSize(uint32_t width, uint32_t height);

		GLuint GetTextureID() const { return m_TextureID; }

		void SetActiveCamera(const std::shared_ptr<Camera>& camera);
		std::shared_ptr<Camera> GetActiveCamera() const { return m_ActiveCamera; }

		std::unordered_map<std::string, std::shared_ptr<Shader>>& GetShaderCache() { return m_ShaderCache; }

		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		void SetupFramebuffer();
	private:
		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;

		GLuint m_FramebufferID;
		GLuint m_TextureID;
		GLuint m_DepthBufferID;

		glm::mat4 m_ProjectionMatrix;
		std::shared_ptr<Object> m_LightObject = nullptr;


		std::shared_ptr<Camera> m_ActiveCamera;
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShaderCache;
	};
}