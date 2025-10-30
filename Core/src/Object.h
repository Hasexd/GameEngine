#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "ECS.h"
#include "Transform.h"
#include "Mesh.h"
#include "UUID.h"

namespace Core
{
	class Object
	{
	public:
		Object(ECS& ecs);
		virtual ~Object() = default;

		void Draw() const;

		template<typename T, typename... Args>
		requires(std::is_base_of_v<Component, T>)
		T& AddComponent(Args&&... args);

		template<typename T>
		T* GetComponent() const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		bool HasComponent() const;

		template<typename... Ts>
		requires((std::is_base_of_v<Component, Ts> && ...))
		bool HasComponents() const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		void RemoveComponent();

		std::string GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		bool IsVisible() const { return m_Visible; }
		void SetVisible(bool visible) { m_Visible = visible; }


		glm::mat4 GetModelMatrix() const;
		glm::mat3 GetNormalMatrix(const glm::mat4& modelMatrix) const;
	private:
		ECS& m_ECS;
		UUID m_UUID;
		std::string m_Name;

		bool m_Visible;
	};


	template<typename T, typename... Args>
	requires(std::is_base_of_v<Component, T>)
	T& Object::AddComponent(Args&&... args)
	{
		return m_ECS.AddComponent<T>(m_UUID, std::forward<Args>(args)...);
	}

	template<typename T>
	T* Object::GetComponent() const
	{
		return m_ECS.GetComponent<T>(m_UUID);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	bool Object::HasComponent() const
	{
		return m_ECS.HasComponent<T>(m_UUID);
	}

	template<typename... Ts>
	requires((std::is_base_of_v<Component, Ts> && ...))
	bool Object::HasComponents() const
	{
		return m_ECS.HasComponents<Ts...>(m_UUID);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	void Object::RemoveComponent()
	{
		m_ECS.RemoveComponent<T>(m_UUID);
	}
}
