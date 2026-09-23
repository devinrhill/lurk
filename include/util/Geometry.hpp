#pragma once

#include <raylib.h>
#include <raymath.h>

struct AABB2D {
    Vector2 center;
    Vector2 halfSize;
};

struct AABB3D {
    Vector3 center;
    Vector3 halfSize;
};

struct Triangle {
	Vector3 a;
	Vector3 b;
	Vector3 c;
};

struct Capsule {
    Vector3 start;
    Vector3 end;
    float radius;
};

struct RightTriangle {
	float base;
	float height;
	int quadrant;
};

struct OBB {
    Vector2 center;
    Vector2 halfSize;
    float rotation;
};

Vector2 quadrantVector2(int quadrant) {
	// accounting for -y for screens
	switch(quadrant) {
	case 1:
		return (Vector2){1.0f, -1.0f};
	case 2:
		return (Vector2){-1.0f, -1.0f};
	case 3:
		return (Vector2){-1.0f, 1.0f};
	case 4:
		return (Vector2){1.0f, 1.0f};
	default:
		return Vector2Zero();
	}
}

void drawRightTriangle(struct RightTriangle rt, Vector2 origin, float scale, Color color) {
	Vector2 q = quadrantVector2(rt.quadrant);

	DrawLine(origin.x, origin.y, origin.x + scale*rt.base*q.x, origin.y, color);
	DrawLine(origin.x, origin.y, origin.x, origin.y + scale*rt.height*q.y, color);
	DrawLine(origin.x, origin.y + scale*rt.height*q.y, origin.x + scale*rt.base*q.x, origin.y, color);
	DrawRectangleLines(origin.x-1, origin.y-1, 6*scale*q.x, 6*scale*q.y, color);
}
