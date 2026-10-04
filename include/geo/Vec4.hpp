// Devin Hill 2026

#pragma once

#include <cmath>
#include <cstdio>
#include <raylib.h>
#include <string>
#include "Vec3.hpp"
#include "../core/Math.hpp"

namespace lvk::geo {

class Vec4 {
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	Vec4() {}

	~Vec4() {}

	Vec4(float x): x{x}, y{x}, z{x}, w{x} {}

	Vec4(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w} {}

	Vec4(Vector4 v): x{v.x}, y{v.y}, z{v.z}, w{v.w} {}

	// unary

	Vec4 operator+() const {
		return Vec4(x, y, z, w);
	}

	Vec4 operator-() const {
		return Vec4(-x, -y, -z, -w);
	}

	// vector arithmetic

	Vec4 operator+(const Vec4& other) const {
		return Vec4(x+other.x, y+other.y, z+other.z, w+other.w);
	}

	Vec4 operator-(const Vec4& other) const {
		return Vec4(x-other.x, y-other.y, z-other.z, w-other.w);
	}

	Vec4 operator*(const Vec4& other) const {
		return Vec4(x*other.x, y*other.y, z*other.z, w*other.w);
	}

	Vec4 operator/(const Vec4& other) const {
		return Vec4(x/other.x, y/other.y, z/other.z, w/other.w);
	}

	// scalar arithmetic

	Vec4 operator*(float scalar) const {
		return Vec4(x*scalar, y*scalar, z*scalar, w*scalar);
	}

	Vec4 operator/(float scalar) const {
		return Vec4(x/scalar, y/scalar, z/scalar, w/scalar);
	}

	friend Vec4 operator*(float scalar, const Vec4& v) {
		return Vec4(v.x*scalar, v.y*scalar, v.z*scalar, v.w*scalar);
	}

	// compound vector arithmetic

	Vec4& operator+=(const Vec4& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	Vec4& operator-=(const Vec4& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	Vec4& operator*=(const Vec4& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}

	Vec4& operator/=(const Vec4& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}

	// compound scalar arithmetic

	Vec4& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	Vec4& operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;

		return *this;
	}

	// comparison

	bool operator==(const Vec4& other) const {
		return x == other.x &&
		       y == other.y &&
		       z == other.z &&
		       w == other.w;
	}

	bool operator!=(const Vec4& other) const {
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

	float dot(const Vec4& other) const {
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

	Vec4 normalized() const {
		float len = length();

		if(len == 0.0f) {
			return Vec4();
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

	float distanceSqr(const Vec4& other) const {
		return (*this - other).lengthSqr();
	}

	float distance(const Vec4& other) const {
		return (*this - other).length();
	}

	// component helpers

	float min() const {
		return std::fmin(std::fmin(x, y), std::fmin(z, w));
	}

	float max() const {
		return std::fmax(std::fmax(x, y), std::fmax(z, w));
	}

	float sum() const {
		return x + y + z + w;
	}

	Vec4 abs() const {
		return Vec4(
			std::fabs(x),
			std::fabs(y),
			std::fabs(z),
			std::fabs(w)
		);
	}

	// leaky
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

	Vector4 raylib() const {
		return (Vector4){x, y, z, w};
	}

	void raylib(Vector4 v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	static Vec4 unit() {
		return Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	static Vec4 zero() {
		return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	static Vec4 one() {
		return Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	Vec3 to3() const {
		return Vec3(x, y, z);
	}

	float comp() const {
		return x * y * z * w;
	}

	float average() const {
		float sum = x + y + z + w;
		return sum / 4.0f;
	}

	bool isNormalized() const {
		return math::eqel(x*x + y+y + z*z + w*w, 1.0f);
	}
};

} // namespace lvk::geo
