#pragma once

#include <cmath>
#include <cstdio>
#include <string>

#include "Vec3.hpp"
#include "Mtx33.hpp"

namespace geo {

class Quat {
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;

	Quat() {}

	~Quat() {}

	Quat(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w} {}

	// unary

	Quat operator+() const {
		return Quat(x, y, z, w);
	}

	Quat operator-() const {
		return Quat(-x, -y, -z, -w);
	}

	// quaternion arithmetic

	Quat operator+(const Quat& other) const {
		return Quat(
			x+other.x,
			y+other.y,
			z+other.z,
			w+other.w
		);
	}

	Quat operator-(const Quat& other) const {
		return Quat(
			x-other.x,
			y-other.y,
			z-other.z,
			w-other.w
		);
	}

	Quat operator*(const Quat& other) const {
		return Quat(
			w*other.x + x*other.w + y*other.z - z*other.y,
			w*other.y - x*other.z + y*other.w + z*other.x,
			w*other.z + x*other.y - y*other.x + z*other.w,
			w*other.w - x*other.x - y*other.y - z*other.z
		);
	}

	// scalar arithmetic

	Quat operator*(float scalar) const {
		return Quat(
			x*scalar,
			y*scalar,
			z*scalar,
			w*scalar
		);
	}

	Quat operator/(float scalar) const {
		return Quat(
			x/scalar,
			y/scalar,
			z/scalar,
			w/scalar
		);
	}

	friend Quat operator*(float scalar, const Quat& q) {
		return Quat(
			q.x*scalar,
			q.y*scalar,
			q.z*scalar,
			q.w*scalar
		);
	}

	// compound quaternion arithmetic

	Quat& operator+=(const Quat& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	Quat& operator-=(const Quat& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	Quat& operator*=(const Quat& other) {
		*this = *this * other;

		return *this;
	}

	// compound scalar arithmetic

	Quat& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	Quat& operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;

		return *this;
	}

	// comparison

	bool operator==(const Quat& other) const {
		return x == other.x &&
		       y == other.y &&
		       z == other.z &&
		       w == other.w;
	}

	bool operator!=(const Quat& other) const {
		return !(*this == other);
	}

	// indexing

	float& operator[](int index) {
		return (&x)[index];
	}

	const float& operator[](int index) const {
		return (&x)[index];
	}

	// math

	float dot(const Quat& other) const {
		return x*other.x +
		       y*other.y +
		       z*other.z +
		       w*other.w;
	}

	float lengthSqr() const {
		return x*x + y*y + z*z + w*w;
	}

	float length() const {
		return std::sqrt(lengthSqr());
	}

	Quat normalized() const {
		float len = length();

		if(len == 0.0f) {
			return Quat();
		}

		return *this / len;
	}

	void normalize() {
		float len = length();

		if(len == 0.0f) {
			return;
		}

		*this /= len;
	}

	Quat conjugate() const {
		return Quat(-x, -y, -z, w);
	}

	Quat inverse() const {
		float lenSqr = lengthSqr();

		if(lenSqr == 0.0f) {
			return Quat();
		}

		return conjugate() / lenSqr;
	}

	void invert() {
		*this = inverse();
	}

	// vector rotation

	Vec3 rotate(const Vec3& v) const {
		Quat qv(v.x, v.y, v.z, 0.0f);
		Quat result = *this * qv * conjugate();

		return Vec3(result.x, result.y, result.z);
	}

	Vec3 operator*(const Vec3& v) const {
		return rotate(v);
	}

	// construction

	static Quat identity() {
		return Quat();
	}

	static Quat fromAxisAngle(const Vec3& axis, float angle) {
		Vec3 n = axis.normalized();

		float half = angle * 0.5f;
		float s = std::sin(half);

		return Quat(
			n.x*s,
			n.y*s,
			n.z*s,
			std::cos(half)
		);
	}

	static Quat fromEuler(float pitch, float yaw, float roll) {
		float cp = std::cos(pitch * 0.5f);
		float sp = std::sin(pitch * 0.5f);
		float cy = std::cos(yaw   * 0.5f);
		float sy = std::sin(yaw   * 0.5f);
		float cr = std::cos(roll  * 0.5f);
		float sr = std::sin(roll  * 0.5f);

		return Quat(
			sr*cp*cy - cr*sp*sy,
			cr*sp*cy + sr*cp*sy,
			cr*cp*sy - sr*sp*cy,
			cr*cp*cy + sr*sp*sy
		);
	}

	// interpolation

	static Quat slerp(const Quat& a, const Quat& b, float t) {
		Quat q1 = a.normalized();
		Quat q2 = b.normalized();

		float d = q1.dot(q2);

		if(d < 0.0f) {
			q2 = -q2;
			d = -d;
		}

		if(d > 0.9995f) {
			return (q1 + (q2-q1)*t).normalized();
		}

		float angle = std::acos(d);
		float sinAngle = std::sin(angle);

		float aWeight = std::sin((1.0f-t)*angle) / sinAngle;
		float bWeight = std::sin(t*angle) / sinAngle;

		return q1*aWeight + q2*bWeight;
	}

	// matrix

	Mtx33 matrix33() const {
		float xx = x*x;
		float yy = y*y;
		float zz = z*z;

		float xy = x*y;
		float xz = x*z;
		float yz = y*z;

		float wx = w*x;
		float wy = w*y;
		float wz = w*z;

		return Mtx33(
			1.0f - 2.0f*(yy + zz),
			2.0f*(xy - wz),
			2.0f*(xz + wy),

			2.0f*(xy + wz),
			1.0f - 2.0f*(xx + zz),
			2.0f*(yz - wx),

			2.0f*(xz - wy),
			2.0f*(yz + wx),
			1.0f - 2.0f*(xx + yy)
		);
	}

	// string

	std::string str() const {
		char out[0x100];

		std::snprintf(
			out,
			0x100,
			"{%.2f, %.2f, %.2f, %.2f}",
			x, y, z, w
		);

		return std::string(out);
	}
};

}
