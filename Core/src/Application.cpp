#include "Application.h"

namespace Core
{
    void Application::Update(const std::shared_ptr<Object>& selectedObject)
    {
        m_Renderer.Render(m_Objects, selectedObject);
        SyncPhysicsWorld();
    }

    void Application::Initialize()
    {
        m_Window.Initialize(1920, 1080, "Engine Window");
        m_Renderer.Initialize();

        m_FileWatcher = FileWatcher(FileUtils::GetCoreProjectDir() + "/shaders", std::chrono::duration<int, std::milli>(500));
       
        m_FileWatcherFuture = std::async(std::launch::async, [this]() -> void
            {
				MonitorShaderChanges();
            });

        AddObject<Cube>();
        AddObject<Cube>();
        AddObject<LightCube>();

        m_Objects[0]->SetName("Cube 1");
        Transform* cubeTransform = m_Objects[0]->GetComponent<Transform>();
        cubeTransform->Z = -5.0f;
        cubeTransform->X = -2.0f;

        m_Objects[1]->SetName("Cube 2");
        cubeTransform = m_Objects[1]->GetComponent<Transform>();
        cubeTransform->Z = -5.0f;
        cubeTransform->X = 2.0f;

		m_Objects[2]->SetName("Light Cube");
		cubeTransform = m_Objects[2]->GetComponent<Transform>();
		cubeTransform->Z = -5.0f;



        SyncPhysicsWorld();

        Mesh::LoadFromObj(FileUtils::GetObjPath("cube"));
    }

    void Application::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_Renderer.SetViewportSize(width, height);
    }

    RaycastHit Application::Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance)
    {
        return m_PhysicsWorld.Raycast(origin, direction, maxDistance);
    }

    RaycastHit Application::ScreenToWorldRaycast(float mouseX, float mouseY, float screenWidth, float screenHeight)
    {
        const std::shared_ptr<Camera> camera = m_Renderer.GetActiveCamera();

        if (!camera)
            return RaycastHit{};

        float x = (2.0f * mouseX) / screenWidth - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / screenHeight;

        glm::mat4 projection = m_Renderer.GetProjectionMatrix();

        glm::mat4 view = camera->GetViewMatrix();

        glm::mat4 invProjection = glm::inverse(projection);
        glm::mat4 invView = glm::inverse(view);

        glm::vec4 clipCoords = glm::vec4(x, y, -1.0f, 1.0f);
        glm::vec4 eyeCoords = invProjection * clipCoords;
        eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

        glm::vec4 worldCoords = invView * eyeCoords;

        glm::vec3 origin = camera->Position;
        glm::vec3 direction = glm::normalize(glm::vec3(worldCoords));

        return Raycast(origin, direction);
    }

    void Application::RemoveObject(const std::shared_ptr<Object>& object)
    {
        auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
        if (it != m_Objects.end())
        {
            m_Objects.erase(it);
            m_PhysicsWorld.UnregisterObject(object);
        }
    }

    void Application::SyncPhysicsWorld()
    {
        m_PhysicsWorld.UpdateObjectList(m_Objects);
    }

    void Application::MonitorShaderChanges()
    {
		m_FileWatcher.Start([this](std::string filePath, Core::FileStatus status) -> void
			{
                if (status == FileStatus::Created)
                {
					LOG_INFO("Shader file created: {}", filePath);

                }
                else if (status == FileStatus::Modified)
                {
                    m_ModifiedShaderFiles.emplace_back(FileUtils::GetFileName(filePath));
                    m_ShadersNeedReload = true;
                }
                else if (status == FileStatus::Deleted)
                {
					LOG_WARN("Shader file deleted: {}", filePath);
                }
			});
    }

    void Application::ReloadShaders()
    {
        if (!m_ShadersNeedReload)
            return;

		LOG_INFO("Shaders needed to reload: ");
        for (const std::string& shaderFile : m_ModifiedShaderFiles)
        {
            for (auto& [name, shader] : m_Renderer.GetShaderCache())
            {
                if (shaderFile == shader->GetFragmentShaderName() || shaderFile == shader->GetVertexShaderName())
                {
					if(shader->Reload())
                        LOG_INFO("{}: OK", name);
                    else
						LOG_ERROR("{}: FAILED", name);
                }
            }
        }
    }

    Application::~Application()
    {
        m_FileWatcher.Stop();
	}
}