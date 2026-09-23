#pragma once

#include <raylib.h>
#include "TaskModel.hpp"

struct TaskChar3D: public TaskModel {
    Vector3 rotation;
    Vector3 velocity;
    Vector3 acceleration;
};
