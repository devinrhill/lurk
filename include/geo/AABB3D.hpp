// Devin Hill 2026

#pragma once

#include <cmath>
#include <cstdio>
#include <raylib.h>
#include <string>

#include "Vec3.hpp"

namespace lvk::geo {

class AABB3D {
public:
	Vec3 min;
	Vec3 max;

	AABB3D() {}

	~AABB3D() {}

	AABB3D(const Vec3& min, const Vec3& max) : min{min}, max{max} {}

	// construction

	static AABB3D fromCenterSize(const Vec3& center, const Vec3& size) {
		Vec3 half = size * 0.5f;

		return AABB3D(
			center - half,
			center + half
		);
	}

	static AABB3D fromCenterHalfSize(
		const Vec3& center,
		const Vec3& halfSize
	) {
		return AABB3D(
			center - halfSize,
			center + halfSize
		);
	}

	// properties

	Vec3 center() const {
		return (min + max) * 0.5f;
	}

	Vec3 size() const {
		return max - min;
	}

	Vec3 halfSize() const {
		return (max - min) * 0.5f;
	}

	float volume() const {
		Vec3 s = size();

		return s.x * s.y * s.z;
	}

	float surfaceArea() const {
		Vec3 s = size();

		return 2.0f * (
			s.x*s.y +
			s.y*s.z +
			s.z*s.x
		);
	}

	// containment

	bool contains(const Vec3& point) const {
		return point.x >= min.x && point.x <= max.x &&
		       point.y >= min.y && point.y <= max.y &&
		       point.z >= min.z && point.z <= max.z;
	}

	bool contains(const AABB3D& other) const {
		return other.min.x >= min.x &&
		       other.min.y >= min.y &&
		       other.min.z >= min.z &&
		       other.max.x <= max.x &&
		       other.max.y <= max.y &&
		       other.max.z <= max.z;
	}

	// intersection

	bool intersects(const AABB3D& other) const {
		return min.x <= other.max.x && max.x >= other.min.x &&
		       min.y <= other.max.y && max.y >= other.min.y &&
		       min.z <= other.max.z && max.z >= other.min.z;
	}

	bool intersects(const Vec3& point) const {
		return contains(point);
	}

	// closest point

	Vec3 closestPoint(const Vec3& point) const {
		return Vec3(
			std::fmax(min.x, std::fmin(point.x, max.x)),
			std::fmax(min.y, std::fmin(point.y, max.y)),
			std::fmax(min.z, std::fmin(point.z, max.z))
		);
	}

	float distanceSqr(const Vec3& point) const {
		return point.distanceSqr(closestPoint(point));
	}

	float distance(const Vec3& point) const {
		return std::sqrt(distanceSqr(point));
	}

	// expansion

	void expand(const Vec3& point) {
		min.x = std::fmin(min.x, point.x);
		min.y = std::fmin(min.y, point.y);
		min.z = std::fmin(min.z, point.z);

		max.x = std::fmax(max.x, point.x);
		max.y = std::fmax(max.y, point.y);
		max.z = std::fmax(max.z, point.z);
	}

	void expand(const AABB3D& other) {
		expand(other.min);
		expand(other.max);
	}

	AABB3D expanded(const Vec3& point) const {
		AABB3D result = *this;
		result.expand(point);

		return result;
	}

	AABB3D expanded(const AABB3D& other) const {
		AABB3D result = *this;
		result.expand(other);

		return result;
	}

	// translation

	AABB3D translated(const Vec3& offset) const {
		return AABB3D(
			min + offset,
			max + offset
		);
	}

	void translate(const Vec3& offset) {
		min += offset;
		max += offset;
	}

	// union

	static AABB3D merge(const AABB3D& a, const AABB3D& b) {
		return AABB3D(
			Vec3(
				std::fmin(a.min.x, b.min.x),
				std::fmin(a.min.y, b.min.y),
				std::fmin(a.min.z, b.min.z)
			),
			Vec3(
				std::fmax(a.max.x, b.max.x),
				std::fmax(a.max.y, b.max.y),
				std::fmax(a.max.z, b.max.z)
			)
		);
	}

	// raylib

	BoundingBox raylib() const {
		return (BoundingBox){
			min.raylib(),
			max.raylib()
		};
	}

	void raylib(BoundingBox box) {
		min.raylib(box.min);
		max.raylib(box.max);
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

} // namespace lvk::geo
