#include "Renderer.h"


namespace Core
{
	void Renderer::Initialize()
	{
		glEnable(GL_DEPTH_TEST);

		m_Shader = std::make_unique<Shader>(FileUtils::GetAssetPath("vertex.glsl"), FileUtils::GetAssetPath("fragment.glsl"));

		m_ViewMatrix = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		m_ProjectionMatrix = glm::perspective(
			glm::radians(45.0f),
			(float)m_ViewportWidth / (float)m_ViewportHeight,
			0.1f,
			100.0f
		);

		SetupFramebuffer();
	}

	void Renderer::Render(const std::vector<Object>& objects)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
		glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Shader->Use();
		m_Shader->SetMatrix4("view", m_ViewMatrix);
		m_Shader->SetMatrix4("projection", m_ProjectionMatrix);

		for (const auto& object : objects)
		{
			const Transform* transform = object.GetComponent<Transform>();
			const Mesh* mesh = object.GetComponent<Mesh>();

			if (transform && mesh)
			{
				glm::mat4 modelMatrix = GetModelMatrix(*transform);
				m_Shader->SetMatrix4("model", modelMatrix);
				m_Shader->SetVec3("color", glm::vec3(1.0f, 0.5f, 0.2f));

				glBindVertexArray(mesh->VAO);
				glDrawElements(GL_TRIANGLES, mesh->IndexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::SetViewportSize(uint32_t width, uint32_t height)
	{

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_ProjectionMatrix = glm::perspective(
			glm::radians(45.0f),
			(float)width / (float)height,
			0.1f,
			100.0f
		);

		if (m_FramebufferID)
		{
			glDeleteFramebuffers(1, &m_FramebufferID);
			glDeleteTextures(1, &m_TextureID);
			glDeleteRenderbuffers(1, &m_DepthBufferID);
		}

		SetupFramebuffer();
	}

	void Renderer::SetupFramebuffer()
	{
		glGenFramebuffers(1, &m_FramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_ViewportWidth, m_ViewportHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

		glGenRenderbuffers(1, &m_DepthBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_ViewportWidth, m_ViewportHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glm::mat4 Renderer::GetModelMatrix(const Transform& transform) const
	{
		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(transform.RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(transform.RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(transform.RotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::translate(model, glm::vec3(transform.X, transform.Y, transform.Z));
		model = model * rotationMatrix;
		model = glm::scale(model, glm::vec3(transform.ScaleX, transform.ScaleY, transform.ScaleZ));

		return model;
	}

	Renderer::~Renderer()
	{
		if (m_FramebufferID)
		{
			glDeleteFramebuffers(1, &m_FramebufferID);
			glDeleteTextures(1, &m_TextureID);
			glDeleteRenderbuffers(1, &m_DepthBufferID);
		}
	}
}