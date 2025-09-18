#pragma once

#include "ECS.h"
#include "Transform.h"

namespace Core
{
	class Object
	{
	public:
		Object(ECS& ecs);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		T* GetComponent() const;

		template<typename T>
		bool HasComponent() const;

		template<typename T>
		void RemoveComponent();
	private:
		ECS& m_ECS;
		std::string m_UUID;
	};


	template<typename T, typename... Args>
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
	bool Object::HasComponent() const
	{
		return m_ECS.HasComponent<T>(m_UUID);
	}

	template<typename T>
	void Object::RemoveComponent()
	{
		m_ECS.RemoveComponent<T>(m_UUID);
	}
}
