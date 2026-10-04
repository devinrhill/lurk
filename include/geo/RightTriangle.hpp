// Devin Hill 2026

#pragma once

#include "Vec2.hpp"

namespace lvk::geo {

struct RightTriangle {
	float base;
	float height;
	int quadrant;

	void draw(struct RightTriangle rt, Vec2 origin, float scale, Color color) {
		Vec2 q = Vec2::quadrant(rt.quadrant);

		DrawLine(origin.x, origin.y, origin.x + scale*rt.base*q.x, origin.y, color);
		DrawLine(origin.x, origin.y, origin.x, origin.y + scale*rt.height*q.y, color);
		DrawLine(origin.x, origin.y + scale*rt.height*q.y, origin.x + scale*rt.base*q.x, origin.y, color);
		DrawRectangleLines(origin.x-1, origin.y-1, 6*scale*q.x, 6*scale*q.y, color);
	}
};

} // namespace lvk::geo
