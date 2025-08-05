#pragma once
#include "Component.h"

namespace Core
{
	struct Transform : Component
	{
		float X = 0.0f, Y = 0.0f;
		float Rotation = 0.0f;
		float ScaleX = 1.0f, ScaleY = 1.0f;


		Transform() = default;
		Transform(float x, float y, float rotation, float scaleX, float scaleY)
			: X(x), Y(y), Rotation(rotation), ScaleX(scaleX), ScaleY(scaleY) {}
		Transform(float x, float y) : X(x), Y(y), Rotation(0.0f), ScaleX(1.0f), ScaleY(1.0f) {}
		virtual ~Transform() = default;
	};
}

