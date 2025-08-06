#pragma once


#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>

#include "Component.h"
#include "UUID.h"
#include "Log.h"

namespace Core
{
	class ECS
	{
	public:
		UUID CreateEntity();

		template<typename T, typename... Args>
		T& AddComponent(UUID entity, Args&&... args);

		template<typename T>
		T* GetComponent(UUID entity);

		template<typename T>
		bool HasComponent(UUID entity) const;

		template<typename T>
		void RemoveComponent(UUID entity);
	private:
		std::unordered_map<std::type_index,
			std::unordered_map<UUID, std::unique_ptr<Core::Component>>> m_Components;

		std::vector<UUID> m_Entities;
	};


	template<typename T, typename... Args>
	T& ECS::AddComponent(UUID entity, Args&&... args)
	{
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		T* componentPtr = component.get();

		m_Components[std::type_index(typeid(T))][entity] = std::move(component);

		return *componentPtr;
	}

	template<typename T>
	T* ECS::GetComponent(UUID entity)
	{
		if (!HasComponent<T>(entity))
			return nullptr;

		return static_cast<T*>(m_Components.find(std::type_index(typeid(T)))->second.find(entity)->second.get());
	}

	template<typename T>
	bool ECS::HasComponent(UUID entity) const
	{
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

		auto typeIt = m_Components.find(std::type_index(typeid(T)));

		if (typeIt == m_Components.end())
			return false;

		return typeIt->second.find(entity) != typeIt->second.end();
	}

	template<typename T>
	void ECS::RemoveComponent(UUID entity)
	{
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

		auto typeIt = m_Components.find(std::type_index(typeid(T)));

		if (typeIt == m_Components.end())
			return;

		typeIt->second.erase(entity);
	}
}

