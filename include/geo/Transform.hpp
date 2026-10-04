#pragma once

#include <raylib.h>

#include "Vec3.hpp"
#include "Quat.hpp"
#include "Mtx44.hpp"

namespace lvk::geo {

class Transform {
public:
	Vec3 position;
	Quat rotation;
	Vec3 scale = Vec3(1.0f, 1.0f, 1.0f);

	Transform() {}

	~Transform() {}

	Transform(
		const Vec3& position,
		const Quat& rotation,
		const Vec3& scale
	) : position{position}, rotation{rotation}, scale{scale} {}

	// matrix

	Mtx44 matrix() const {
		return Mtx44::translation(position.x, position.y, position.z)
	     	* Mtx44::rotation(rotation)
	     	* Mtx44::scale(scale.x, scale.y, scale.z);
	}

	// transform

	Vec3 transformPoint(const Vec3& point) const {
		return rotation.rotate(point * scale) + position;
	}

	Vec3 transformVector(const Vec3& vector) const {
		return rotation.rotate(vector * scale);
	}

	Vec3 inversePoint(const Vec3& point) const {
		Vec3 result = point - position;
		result = rotation.conjugate().rotate(result);

		return result / scale;
	}

	Vec3 inverseVector(const Vec3& vector) const {
		Vec3 result = rotation.conjugate().rotate(vector);

		return result / scale;
	}

	// raylib

	Vector3 transformPointRaylib(const Vector3& point) const {
		return transformPoint(Vec3(point.x, point.y, point.z)).raylib();
	}

	Vector3 transformVectorRaylib(const Vector3& vector) const {
		return transformVector(Vec3(vector.x, vector.y, vector.z)).raylib();
	}
};

} // namespace lvk::geo
