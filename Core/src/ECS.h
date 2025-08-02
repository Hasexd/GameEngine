#pragma once

#include "Component.h"

#include <unordered_map>
#include <typeindex>
#include <vector>

class ECS
{
public:
	Entity CreateEntity();

	template<typename T, typename... Args>
	T& AddComponent(Entity entity, Args&&... args);

	template<typename T>
	T* GetComponent(Entity entity);
private:
	std::unordered_map<std::type_index,
		std::unordered_map<Entity, std::unique_ptr<Component>>> m_Components;

	std::vector<Entity> m_Entities;
};


template<typename T, typename... Args>
T& ECS::AddComponent(Entity entity, Args&&... args)
{
	static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

	auto component = std::make_unique<T>(std::forward<Args>(args)...);
	T* componentPtr = component.get();

	m_Components[std::type_index(typeid(T))][entity] = std::move(component);

	return *componentPtr;
}

template<typename T>
T* ECS::GetComponent(Entity entity)
{
	auto typeIt = m_Components.find(std::type_index(typeid(T)));

	if (typeIt == m_Components.end())
		return nullptr;

	auto entityIt = typeIt->second.find(entity);
	if (entityIt == typeIt->second.end())
		return nullptr;

	return static_cast<T*>(entityIt->second.get());
}