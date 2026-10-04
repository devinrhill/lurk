// Devin Hill 2026

#pragma once

#include <raylib.h>
#include "../TaskModel.hpp"

namespace lvk {

struct TaskChar3D: public TaskModel {
	Vec3 lastPosition;
    Vec3 rotation;
    Vec3 velocity;
    Vec3 lastVelocity;
    Vec3 acceleration;
};

} // namespace lvk
