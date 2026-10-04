// Devin Hill 2026

#pragma once

#include <raylib.h>
#include "../geo/Vec2.hpp"
#include "Task.hpp"

using namespace lvk::geo;

namespace lvk {

struct TaskChar2D : public Task {
	Vec2 position;
	Vec2 lastPosition;
	Vec2 rotation;
	Vec2 scale;
	Vec2 velocity;
	Vec2 lastVelocity;
	Vec2 acceleration;
};

} // namespace lvk
