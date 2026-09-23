#ifndef TASKCHAR2D_HPP
#define TASKCHAR2D_HPP

#include "Task.hpp"
#include <raylib.h>

struct TaskChar2D : public Task {
  Vector2 trans;
  Vector2 lastTrans;
  Vector2 rotate;
  Vector2 scale;
  Vector2 lastVeloc;
  Vector2 veloc;
  Vector2 accel;
};

#endif // TASKCHAR2D_HPP
