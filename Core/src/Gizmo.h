#pragma once

#include "Object.h"
#include "Mesh.h"
#include "FileUtils.h"
#include "Log.h"
#include <vector>

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

	class Gizmo final : public Object
	{
	public:
		~Gizmo() override = default;

		[[nodiscard]] glm::mat4 GetRotation(const glm::mat4& modelMatrix) const;
		[[nodiscard]] glm::vec3 GetColor() const;

		[[nodiscard]] GizmoType GetType() const { return m_Type; }
		[[nodiscard]] GizmoAxis GetAxis() const { return m_Axis; }

		friend class Application;
	private:
		Gizmo(ECS& ecs, GizmoType gizmoType, GizmoAxis gizmoAxis);
		[[nodiscard]] static Gizmo Create(ECS& ecs, GizmoType gizmoType, GizmoAxis gizmoAxis);
	private:
		GizmoType m_Type = GizmoType::None;
		GizmoAxis m_Axis = GizmoAxis::None;
	};
}