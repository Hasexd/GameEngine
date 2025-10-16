#pragma once

#include "Object.h"
#include "Mesh.h"
#include "FileUtils.h"
#include "Log.h"

namespace Core
{
	enum class GizmoType : uint8_t
	{
		None = 0,
		Position,
		Rotation,
		Scale
	};

	enum class GizmoAxis : uint8_t
	{
		None = 0,
		X,
		Y,
		Z
	};

	class Gizmo : public Object
	{
	public:
		Gizmo(ECS& ecs, GizmoType gizmoType, GizmoAxis gizmoAxis);
		~Gizmo() override = default;

		glm::mat4 GetRotation(const glm::mat4& modelMatrix) const;
		glm::vec3 GetColor() const;

		GizmoType GetType() const { return m_Type; }
		GizmoAxis GetAxis() const { return m_Axis; }

		friend class Application;
	private:

	private:
		GizmoType m_Type = GizmoType::None;
		GizmoAxis m_Axis = GizmoAxis::None;
	};
}