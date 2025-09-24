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
		std::string CreateEntity();

		template<typename T, typename... Args>
		requires(std::is_base_of_v<Component, T>)
		T& AddComponent(std::string entity, Args&&... args);

		template<typename T>
		T* GetComponent(std::string entity);

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		bool HasComponent(std::string entity) const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		void RemoveComponent(std::string entity);
	private:
		std::unordered_map<std::type_index,
			std::unordered_map<std::string, std::unique_ptr<Core::Component>>> m_Components;

		std::vector<std::string> m_Entities;
	};


	template<typename T, typename... Args>
	requires(std::is_base_of_v<Component, T>)
	T& ECS::AddComponent(std::string entity, Args&&... args)
	{
		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		T* componentPtr = component.get();

		m_Components[std::type_index(typeid(T))][entity] = std::move(component);

		return *componentPtr;
	}

	template<typename T>
	T* ECS::GetComponent(std::string entity)
	{
		if (!HasComponent<T>(entity))
			return nullptr;

		return static_cast<T*>(m_Components.find(std::type_index(typeid(T)))->second.find(entity)->second.get());
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	bool ECS::HasComponent(std::string entity) const
	{
		auto typeIt = m_Components.find(std::type_index(typeid(T)));

		if (typeIt == m_Components.end())
			return false;

		return typeIt->second.find(entity) != typeIt->second.end();
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	void ECS::RemoveComponent(std::string entity)
	{
		auto typeIt = m_Components.find(std::type_index(typeid(T)));

		if (typeIt == m_Components.end())
			return;

		typeIt->second.erase(entity);
	}
}

