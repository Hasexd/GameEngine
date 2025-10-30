#pragma once


#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>
#include <string>

#include "Component.h"
#include "UUID.h"
#include "Log.h"

namespace Core
{
	class ECS
	{
	public:
		template<typename T, typename... Args>
		requires(std::is_base_of_v<Component, T>)
		T& AddComponent(UUID entity, Args&&... args);

		template<typename T>
		T* GetComponent(UUID entity);

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		bool HasComponent(UUID entity) const;

		template<typename... Ts>
		requires((std::is_base_of_v<Component, Ts> && ...))
		bool HasComponents(UUID entity) const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		void RemoveComponent(UUID entity);
	private:
		std::unordered_map<std::type_index,
			std::unordered_map<UUID, std::unique_ptr<Core::Component>>> m_Components;
	};


	template<typename T, typename... Args>
	requires(std::is_base_of_v<Component, T>)
	T& ECS::AddComponent(UUID entity, Args&&... args)
	{
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
	requires(std::is_base_of_v<Component, T>)
	bool ECS::HasComponent(UUID entity) const
	{
		auto typeIt = m_Components.find(std::type_index(typeid(T)));

		if (typeIt == m_Components.end())
			return false;

		return typeIt->second.contains(entity);
	}

	template<typename... Ts>
	requires((std::is_base_of_v<Component, Ts> && ...))
	bool ECS::HasComponents(UUID entity) const
	{
		return (HasComponent<Ts>(entity) && ...);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	void ECS::RemoveComponent(UUID entity)
	{
		auto typeIt = m_Components.find(std::type_index(typeid(T)));

		if (typeIt == m_Components.end())
			return;

		typeIt->second.erase(entity);
	}
}

