// Devin Hill 2026

#pragma once

#include "Vec2.hpp"

namespace lvk::geo {

struct OBB {
	Vec2 center;
	Vec2 halfSize;
	float rotate;
};

} // namespace lvk::geo
