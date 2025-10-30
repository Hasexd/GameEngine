#include "Object.h"


namespace Core
{
	Object::Object(ECS& ecs):
		m_ECS(ecs), m_UUID(UUID()), m_Name("Object"), m_Visible(true)
	{
		AddComponent<Transform>();
	}

	void Object::Draw() const
	{
		if (!HasComponent<Mesh>())
		{
			LOG_WARN("Object {} UUID: {} has no mesh component! Draw() returns early.", m_Name, m_UUID.ToString());
			return;
		}

		Mesh* mesh = GetComponent<Mesh>();

		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES, mesh->IndexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glm::mat4 Object::GetModelMatrix() const
	{
		if (!HasComponent<Transform>())
		{
			LOG_WARN("Object {} UUID: {} has no transform component! GetModelMatrix() returns 0.", m_Name, m_UUID.ToString());
			return glm::mat4(0.0f);
		}

		Transform* transform = GetComponent<Transform>();

		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(transform->RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(transform->RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(transform->RotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::translate(model, glm::vec3(transform->X, -transform->Y, transform->Z));
		model = model * rotationMatrix;
		model = glm::scale(model, glm::vec3(transform->ScaleX, transform->ScaleY, transform->ScaleZ));

		return model;
	}

	glm::mat3 Object::GetNormalMatrix(const glm::mat4& modelMatrix) const
	{
		return glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
	}

}
