#pragma once

// Devin Hill 2026

#include <cmath>

namespace util {

float stepSmooth1(float t) {
	return t * t * (3.0f - 2.0f * t);
}

float stepSmooth2(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float stepEaseIn(float t) {
	return t * t;
}

float stepEaseOut(float t) {
	return 1.0f - (1.0f - t) * (1.0f - t);
}

float stepExp(float t) {
	return t = 1.0f - std::expf(-5.0f * t);
}

float stepSine(float t) {
	return std::powf(std::sinf(t * (M_PI * 0.5f)), 2.0f);
}

float fhermite(float p0, float m0, float p1, float m1, float t) {
	float t2 = t * t;
	float t3 = t2 * t;

	float h00 =  2.0f*t3 - 3.0f*t2 + 1.0f;
	float h10 = t3 - 2.0f*t2 + t;
	float h01 = -2.0f*t3 + 3.0f*t2;
	float h11 = t3 - t2;

	return h00*p0 + h10*m0 + h01*p1 + h11*m1;
}

} // namespace util
