#pragma once
#include "Component.h"

namespace Core
{
	struct Transform : Component
	{
		float X = 0.0f, Y = 0.0f, Z = 0.0f;
		float Rotation = 0.0f;
		float ScaleX = 1.0f, ScaleY = 1.0f, ScaleZ = 1.0f;


		Transform() = default;

		Transform(float x, float y, float z, float rotation, float scaleX, float scaleY, float scaleZ)
			: X(x), Y(y), Z(z), Rotation(rotation), ScaleX(scaleX), ScaleY(scaleY), ScaleZ(scaleZ) {}

		Transform(float x, float y, float z):
			X(x), Y(y), Z(z) {}

		virtual ~Transform() = default;
	};
}

