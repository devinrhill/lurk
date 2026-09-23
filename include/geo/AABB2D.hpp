#pragma once

#include <cmath>
#include <cstdio>
#include <raylib.h>
#include <string>

#include "Vec2.hpp"

namespace geo {

class AABB2D {
public:
	Vec2 min;
	Vec2 max;

	AABB2D() {}

	~AABB2D() {}

	AABB2D(const Vec2& min, const Vec2& max) : min{min}, max{max} {}

	// construction

	static AABB2D fromCenterSize(const Vec2& center, const Vec2& size) {
		Vec2 half = size * 0.5f;

		return AABB2D(
			center - half,
			center + half
		);
	}

	static AABB2D fromCenterHalfSize(
		const Vec2& center,
		const Vec2& halfSize
	) {
		return AABB2D(
			center - halfSize,
			center + halfSize
		);
	}

	// properties

	Vec2 center() const {
		return (min + max) * 0.5f;
	}

	Vec2 size() const {
		return max - min;
	}

	Vec2 halfSize() const {
		return (max - min) * 0.5f;
	}

	float area() const {
		Vec2 s = size();

		return s.x * s.y;
	}

	// containment

	bool contains(const Vec2& point) const {
		return point.x >= min.x && point.x <= max.x &&
		       point.y >= min.y && point.y <= max.y;
	}

	bool contains(const AABB2D& other) const {
		return other.min.x >= min.x &&
		       other.min.y >= min.y &&
		       other.max.x <= max.x &&
		       other.max.y <= max.y;
	}

	// intersection

	bool intersects(const AABB2D& other) const {
		return min.x <= other.max.x && max.x >= other.min.x &&
		       min.y <= other.max.y && max.y >= other.min.y;
	}

	bool intersects(const Vec2& point) const {
		return contains(point);
	}

	// closest point

	Vec2 closestPoint(const Vec2& point) const {
		return Vec2(
			std::fmax(min.x, std::fmin(point.x, max.x)),
			std::fmax(min.y, std::fmin(point.y, max.y))
		);
	}

	float distanceSqr(const Vec2& point) const {
		return point.distanceSqr(closestPoint(point));
	}

	float distance(const Vec2& point) const {
		return std::sqrt(distanceSqr(point));
	}

	// expansion

	void expand(const Vec2& point) {
		min.x = std::fmin(min.x, point.x);
		min.y = std::fmin(min.y, point.y);

		max.x = std::fmax(max.x, point.x);
		max.y = std::fmax(max.y, point.y);
	}

	void expand(const AABB2D& other) {
		expand(other.min);
		expand(other.max);
	}

	AABB2D expanded(const Vec2& point) const {
		AABB2D result = *this;
		result.expand(point);

		return result;
	}

	AABB2D expanded(const AABB2D& other) const {
		AABB2D result = *this;
		result.expand(other);

		return result;
	}

	// translation

	AABB2D translated(const Vec2& offset) const {
		return AABB2D(
			min + offset,
			max + offset
		);
	}

	void translate(const Vec2& offset) {
		min += offset;
		max += offset;
	}

	// union

	static AABB2D merge(const AABB2D& a, const AABB2D& b) {
		return AABB2D(
			Vec2(
				std::fmin(a.min.x, b.min.x),
				std::fmin(a.min.y, b.min.y)
			),
			Vec2(
				std::fmax(a.max.x, b.max.x),
				std::fmax(a.max.y, b.max.y)
			)
		);
	}

	// raylib

	Rectangle raylib() const {
		return (Rectangle){
			min.x,
			min.y,
			max.x - min.x,
			max.y - min.y
		};
	}

	void raylib(Rectangle rect) {
		min.x = rect.x;
		min.y = rect.y;

		max.x = rect.x + rect.width;
		max.y = rect.y + rect.height;
	}

	// string

	std::string str() const {
		char out[0x100];

		std::snprintf(
			out,
			0x100,
			"{min: %s, max: %s}",
			min.str().c_str(),
			max.str().c_str()
		);

		return std::string(out);
	}
};

}
