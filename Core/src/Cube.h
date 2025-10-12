#pragma once

#include "Object.h"
#include "Mesh.h"
#include "FileUtils.h"

namespace Core
{
	class Cube : public Object
	{
	public:
		Cube(ECS& ecs);
	};
}