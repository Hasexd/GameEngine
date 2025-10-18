#pragma once

#include <glm.hpp>
#include <ext/matrix_transform.hpp>

#include <vector>
#include <memory>
#include "Object.h"
#include "ECS.h"

namespace Core
{

	struct Ray
	{
		glm::vec3 Origin;
		glm::vec3 Direction;
	};

	struct RaycastHit
	{
		bool Hit = false;

		glm::vec3 Point = glm::vec3(0.0f);
		glm::vec3 Normal = glm::vec3(0.0f);

		float Distance = 0.0f;

		std::shared_ptr<Object> HitObject = nullptr;
	};

	class PhysicsWorld
	{
	public:
		PhysicsWorld() = default;

		void RegisterObject(const std::shared_ptr<Object>& object);
		void UnregisterObject(const std::shared_ptr<Object>& object);
		void UpdateObjectList(const std::vector<std::shared_ptr<Object>>& objects);

		RaycastHit Raycast(const Ray& ray, float maxDistance = FLT_MAX);

		std::vector<std::shared_ptr<Object>> GetObjectsAtPoint(const glm::vec3& point, float radius = 0.1f);

	private:
		std::vector<std::shared_ptr<Object>> m_RegisteredObjects;

		RaycastHit TestRayAABB(const Ray& ray, const std::shared_ptr<Object>& object, float maxDistance);
		bool RayAABBIntersection(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax, float& distance);
	};
}