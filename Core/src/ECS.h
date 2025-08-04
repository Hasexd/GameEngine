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

	template<typename T>
	bool HasComponent(Entity entity) const;

	template<typename T>
	void RemoveComponent(Entity entity);
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
	if (!HasComponent<T>(entity))
		return nullptr;

	return static_cast<T*>(m_Components.find(std::type_index(typeid(T)))->second.find(entity)->second.get());
}

template<typename T>
bool ECS::HasComponent(Entity entity) const
{
	auto typeIt = m_Components.find(std::type_index(typeid(T)));

	if (typeIt == m_Components.end())
		return false;

	return typeIt->second.find(entity) != typeIt->second.end();
}

template<typename T>
void ECS::RemoveComponent(Entity entity)
{
	auto typeIt = m_Components.find(std::type_index(typeid(T)));

	if(typeIt == m_Components.end())
		return;

	typeIt->second.erase(entity);
}