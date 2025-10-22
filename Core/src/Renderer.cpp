#include "Renderer.h"


namespace Core
{
	void Renderer::Initialize()
	{
		glEnable(GL_DEPTH_TEST);

		auto objectShader = std::make_unique<Shader>(FileUtils::GetShaderPath("objectVert.glsl"), FileUtils::GetShaderPath("objectFrag.glsl"));
		auto lightShader = std::make_unique<Shader>(FileUtils::GetShaderPath("objectVert.glsl"), FileUtils::GetShaderPath("lightFrag.glsl"));
		auto gizmoShader = std::make_unique<Shader>(FileUtils::GetShaderPath("gizmoVert.glsl"), FileUtils::GetShaderPath("gizmoFrag.glsl"));

		m_ShaderCache["object"] = std::move(objectShader);
		m_ShaderCache["light"] = std::move(lightShader);
		m_ShaderCache["gizmo"] = std::move(gizmoShader);

		m_ProjectionMatrix = glm::perspective(
			45.0f,
			(float)m_ViewportWidth / (float)m_ViewportHeight,
			0.1f,
			100.0f
		);

		SetupFramebuffer();
	}

	void Renderer::Render(const std::vector<std::shared_ptr<Object>>& objects, const std::shared_ptr<Object>& selectedObject)
	{
		if (!m_ActiveCamera)
			return;

		m_ProjectionMatrix = glm::perspective(
			glm::radians(m_ActiveCamera->FieldOfView),
			(float)m_ViewportWidth / (float)m_ViewportHeight,
			0.1f,
			100.0f
		);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
		glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		auto drawableObjects = objects | std::ranges::views::filter([](std::shared_ptr<Object> object) {
			return object->HasComponents<Transform, Mesh>() && object->IsVisible();
		});

		for (const auto& object : drawableObjects)
		{
			if (std::dynamic_pointer_cast<LightCube>(object))
			{
				RenderLight(object);
			} 
			else
			{
				RenderObject(object);
			}
		}
		if(!selectedObject)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::RenderObject(const std::shared_ptr<Object>& object)
	{
		const auto& objShader = m_ShaderCache.at("object");

		if (!objShader)
		{
			LOG_CRITICAL("Object shader not found in the cache, unable to render.");
			return;
		}

		glm::mat4 modelMatrix = object->GetModelMatrix();
		glm::vec3 lightPos = glm::vec3(0);

		if (m_LightObject)
		{
			if (m_LightObject->HasComponent<Transform>())
			{
				const Transform* lightTransform = m_LightObject->GetComponent<Transform>();

				lightPos = glm::vec3(lightTransform->X, lightTransform->Y, lightTransform->Z);
			}
		}

		objShader->Use();

		objShader->SetMatrix4("view", m_ActiveCamera->GetViewMatrix());
		objShader->SetMatrix4("projection", m_ProjectionMatrix);
		objShader->SetMatrix4("model", modelMatrix);
		objShader->SetMatrix3("normal", object->GetNormalMatrix(modelMatrix));

		objShader->SetVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
		objShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		objShader->SetVec3("lightPos", lightPos);
		objShader->SetVec3("viewPos", m_ActiveCamera->Position);

		object->Draw();
	}

	void Renderer::RenderLight(const std::shared_ptr<Object>& light)
	{
		const auto& lightShader = m_ShaderCache.at("light");

		if (!lightShader)
		{
			LOG_CRITICAL("Light shader not found in the cache, unable to render.");
			return;
		}

		m_LightObject = light;

		glm::mat4 modelMatrix = light->GetModelMatrix();

		lightShader->Use();

		lightShader->SetMatrix4("view", m_ActiveCamera->GetViewMatrix());
		lightShader->SetMatrix4("projection", m_ProjectionMatrix);
		lightShader->SetMatrix4("model", modelMatrix);
		lightShader->SetMatrix3("normal", light->GetNormalMatrix(modelMatrix));

		light->Draw();
	}

	
	void Renderer::RenderGizmos(const std::vector<std::shared_ptr<Gizmo>>& gizmos, const std::shared_ptr<Object>& selectedObject)
	{
		if (!m_ActiveCamera || !selectedObject)
			return;

		const auto& gizmoShader = m_ShaderCache.at("gizmo");

		for (const auto& gizmo : gizmos)
		{

			if (!gizmoShader)
			{
				LOG_CRITICAL("Axis shader not found in the cache, unable to render.");
				return;
			}

			Transform* selectedTransform = selectedObject->GetComponent<Transform>();
			glm::vec3 selectedPos = selectedTransform->GetPosition();

			glm::vec3 cameraPos = m_ActiveCamera->Position;
			float distance = glm::length(cameraPos - selectedPos);

			float constantScale = distance * 0.05f;

			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, selectedPos);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(constantScale));
			modelMatrix = gizmo->GetRotation(modelMatrix);

			gizmoShader->Use();

			gizmoShader->SetMatrix4("view", m_ActiveCamera->GetViewMatrix());
			gizmoShader->SetMatrix4("projection", m_ProjectionMatrix);
			gizmoShader->SetMatrix4("model", modelMatrix);

			gizmoShader->SetVec3("axisColor", gizmo->GetColor());

			glDepthFunc(GL_ALWAYS);
			gizmo->Draw();
			glDepthFunc(GL_LESS);
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
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_ViewportWidth, m_ViewportHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::SetActiveCamera(const std::shared_ptr<Camera>& camera)
	{
		m_ActiveCamera = camera;

		m_ProjectionMatrix = glm::perspective(
			glm::radians(m_ActiveCamera->FieldOfView),
			(float)m_ViewportWidth / (float)m_ViewportHeight,
			0.1f,
			100.0f
		);
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