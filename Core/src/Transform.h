#pragma once
#include "Component.h"

struct Transform : Component
{
	float X = 0.0f, Y = 0.0f;
	float Rotation = 0.0f;
	float ScaleX = 1.0f, ScaleY = 1.0f;

	virtual ~Transform() = default;
};