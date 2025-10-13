#pragma once

#include "Object.h"
#include "Mesh.h"
#include "FileUtils.h"

namespace Core
{
	class AxisArrow : public Object
	{
	public:
		AxisArrow(ECS& ecs);
		~AxisArrow() override = default;

	};
}