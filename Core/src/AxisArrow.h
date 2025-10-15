#pragma once

#include "Object.h"
#include "Mesh.h"
#include "FileUtils.h"
#include "Log.h"

namespace Core
{
	class AxisArrow : public Object
	{
	public:
		AxisArrow(ECS& ecs);
		~AxisArrow() override = default;

	private:
		static inline uint32_t Count;
	};
}