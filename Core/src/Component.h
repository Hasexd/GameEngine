#pragma once

#include "UUID.h"

using Entity = std::string;

struct Component
{
	virtual ~Component() = default;
};