#include "Physics.h"

namespace Core
{
	void PhysicsWorld::RegisterObject(const std::shared_ptr<Object>& object)
	{
		if (!object)
			return;

		auto it = std::find(m_RegisteredObjects.begin(), m_RegisteredObjects.end(), object);
		if(it == m_RegisteredObjects.end())
			m_RegisteredObjects.push_back(object);
	}

	void PhysicsWorld::UnregisterObject(const std::shared_ptr<Object>& object)
	{
		auto it = std::find(m_RegisteredObjects.begin(), m_RegisteredObjects.end(), object);
		if (it != m_RegisteredObjects.end())
		{
			m_RegisteredObjects.erase(it);
		}
	}

	void PhysicsWorld::UpdateObjectList(const std::vector<std::shared_ptr<Object>>& objects)
	{
		m_RegisteredObjects = objects;
	}

	RaycastHit PhysicsWorld::Raycast(const Ray& ray, float maxDistance)
	{
		RaycastHit closestHit;
		float closestDistance = maxDistance;

		for (const auto& obj : m_RegisteredObjects)
		{
			if (!obj)
				continue;

			RaycastHit hit = TestRayAABB(ray, obj, maxDistance);

			if (hit.Hit && hit.Distance < closestDistance)
			{
				closestHit = hit;
				closestDistance = hit.Distance;
			}
		}

		return closestHit;
	}

	std::vector<std::shared_ptr<Object>> PhysicsWorld::GetObjectsAtPoint(const glm::vec3& point, float radius)
	{
		std::vector<std::shared_ptr<Object>> objectsAtPoint;

		for (const auto& obj : m_RegisteredObjects)
		{
			if (!obj)
				continue;

			Transform* transform = obj->GetComponent<Transform>();

			if (!transform)
				continue;

			glm::vec3 objPos(transform->X, -transform->Y, transform->Z);
			float distance = glm::length(point - objPos);

			if(distance <= radius)
				objectsAtPoint.push_back(obj);
		}

		return objectsAtPoint;
	}

	RaycastHit PhysicsWorld::TestRayAABB(const Ray& ray, const std::shared_ptr<Object>& object, float maxDistance)
	{
		RaycastHit hit;

		Transform* transform = object->GetComponent<Transform>();

		if (!transform)
			return hit;

		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(transform->RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(transform->RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(transform->RotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::translate(model, glm::vec3(transform->X, -transform->Y, transform->Z));
		model = model * rotationMatrix;
		model = glm::scale(model, glm::vec3(transform->ScaleX, transform->ScaleY, transform->ScaleZ));

		glm::mat4 invModel = glm::inverse(model);

		glm::vec4 localOrigin = invModel * glm::vec4(ray.Origin, 1.0f);
		glm::vec4 localDirection = invModel * glm::vec4(ray.Direction, 0.0f);

		glm::vec3 boxMin = glm::vec3(-0.5f, -0.5f, -0.5f);
		glm::vec3 boxMax = glm::vec3(0.5f, 0.5f, 0.5f);

		float distance;
		if (RayAABBIntersection({ glm::vec3(localOrigin), glm::normalize(glm::vec3(localDirection)) }, boxMin, boxMax, distance))
		{
			glm::vec3 worldDirection = glm::normalize(ray.Direction);
			glm::vec3 localDir = glm::normalize(glm::vec3(localDirection));
			float scale = glm::length(glm::vec3(localDirection));
			distance *= scale;

			if (distance <= maxDistance && distance >= 0.0f)
			{
				hit.Hit = true;
				hit.Distance = distance;
				hit.Point = ray.Origin + ray.Direction * distance;
				hit.Object = object;

				glm::vec3 localHit = glm::vec3(localOrigin) + localDir * (distance / scale);
				glm::vec3 absLocalHit = glm::abs(localHit);

				glm::vec3 localNormal;
				if (absLocalHit.x > absLocalHit.y && absLocalHit.x > absLocalHit.z)
					localNormal = glm::vec3(localHit.x > 0 ? 1 : -1, 0, 0);
				else if (absLocalHit.y > absLocalHit.z)
					localNormal = glm::vec3(0, localHit.y > 0 ? 1 : -1, 0);
				else
					localNormal = glm::vec3(0, 0, localHit.z > 0 ? 1 : -1);

				hit.Normal = glm::normalize(glm::vec3(glm::transpose(invModel) * glm::vec4(localNormal, 0.0f)));
			}
		}

		return hit;
	}

	bool PhysicsWorld::RayAABBIntersection(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax, float& distance)
	{
		glm::vec3 invDir = 1.0f / ray.Direction;
		glm::vec3 t1 = (boxMin - ray.Origin) * invDir;
		glm::vec3 t2 = (boxMax - ray.Origin) * invDir;

		glm::vec3 tMin = glm::min(t1, t2);
		glm::vec3 tMax = glm::max(t1, t2);

		float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
		float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

		if (tNear > tFar || tFar < 0.0f)
			return false;

		distance = tNear < 0.0f ? tFar : tNear;
		return true;
	}
}