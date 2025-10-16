#include "Gizmo.h"

namespace Core
{
	Gizmo::Gizmo(ECS& ecs, GizmoType gizmoType, GizmoAxis gizmoAxis):
		Object(ecs), m_Type(gizmoType), m_Axis(gizmoAxis)
	{
		MeshData meshData = {};
		switch (m_Type)
		{
			case GizmoType::Position:
				meshData = Mesh::LoadFromObj(FileUtils::GetObjPath("PositionGizmo"));
				break;
			case GizmoType::Rotation:
				meshData = Mesh::LoadFromObj(FileUtils::GetObjPath("RotationGizmo"));
				break;
			case GizmoType::Scale:
				meshData = Mesh::LoadFromObj(FileUtils::GetObjPath("ScaleGizmo"));
				break;
			case GizmoType::None:
			default:
				break;
		}

		AddComponent<Mesh>(meshData);
		SetVisible(false);
	}

	glm::mat4 Gizmo::GetRotation(const glm::mat4& modelMatrix) const
	{
		switch (m_Axis)
		{
		case GizmoAxis::X:
			return glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		case GizmoAxis::Y:
			return glm::rotate(modelMatrix, glm::radians(270.f), glm::vec3(1.0f, 0.0f, 0.0f));
		case GizmoAxis::Z:
			return glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		case GizmoAxis::None:
			default:
				return modelMatrix;
		}
	}

	glm::vec3 Gizmo::GetColor() const
	{
		switch(m_Axis)
		{
		case GizmoAxis::X:
			return { 1.0f, 0.0f, 0.0f };
		case GizmoAxis::Y:
			return { 0.0f, 1.0f, 0.0f };
		case GizmoAxis::Z:
			return { 0.0f, 0.0f, 1.0f };
		case GizmoAxis::None:
		default:
			return { 1.0f, 1.0f, 1.0f };
		}
	}
}