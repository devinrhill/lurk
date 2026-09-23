#pragma once

#include <cmath>
#include <cstdio>
#include <raylib.h>
#include <string>

namespace geo {

class Vec3 {
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Vec3() {}

	~Vec3() {}

	Vec3(float x, float y, float z) : x{x}, y{y}, z{z} {}

	Vec3(Vector3 v): x{v.x}, y{v.y}, z{v.z} {}

	// unary

	Vec3 operator+() const {
		return Vec3(x, y, z);
	}

	Vec3 operator-() const {
		return Vec3(-x, -y, -z);
	}

	// vector arithmetic

	Vec3 operator+(const Vec3& other) const {
		return Vec3(x+other.x, y+other.y, z+other.z);
	}

	Vec3 operator-(const Vec3& other) const {
		return Vec3(x-other.x, y-other.y, z-other.z);
	}

	Vec3 operator*(const Vec3& other) const {
		return Vec3(x*other.x, y*other.y, z*other.z);
	}

	Vec3 operator/(const Vec3& other) const {
		return Vec3(x/other.x, y/other.y, z/other.z);
	}

	// scalar arithmetic

	Vec3 operator*(float scalar) const {
		return Vec3(x*scalar, y*scalar, z*scalar);
	}

	Vec3 operator/(float scalar) const {
		return Vec3(x/scalar, y/scalar, z/scalar);
	}

	friend Vec3 operator*(float scalar, const Vec3& v) {
		return Vec3(v.x*scalar, v.y*scalar, v.z*scalar);
	}

	// compound vector arithmetic

	Vec3& operator+=(const Vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vec3& operator-=(const Vec3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	Vec3& operator*=(const Vec3& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	Vec3& operator/=(const Vec3& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}

	// compound scalar arithmetic

	Vec3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vec3& operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	// comparison

	bool operator==(const Vec3& other) const {
		return x == other.x &&
		       y == other.y &&
		       z == other.z;
	}

	bool operator!=(const Vec3& other) const {
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

	float dot(const Vec3& other) const {
		return x*other.x +
		       y*other.y +
		       z*other.z;
	}

	Vec3 cross(const Vec3& other) const {
		return Vec3(
			y*other.z - z*other.y,
			z*other.x - x*other.z,
			x*other.y - y*other.x
		);
	}

	float lengthSqr() const {
		return x*x + y*y + z*z;
	}

	float length() const {
		return std::sqrt(lengthSqr());
	}

	Vec3 normalized() const {
		float len = length();

		if(len == 0.0f) {
			return Vec3();
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

	float distanceSqr(const Vec3& other) const {
		return (*this - other).lengthSqr();
	}

	float distance(const Vec3& other) const {
		return (*this - other).length();
	}

	// component helpers

	float min() const {
		return std::fmin(std::fmin(x, y), z);
	}

	float max() const {
		return std::fmax(std::fmax(x, y), z);
	}

	float sum() const {
		return x + y + z;
	}

	Vec3 abs() const {
		return Vec3(
			std::fabs(x),
			std::fabs(y),
			std::fabs(z)
		);
	}

	// string

	std::string str() const {
		char out[0x100];

		std::snprintf(
			out,
			0x100,
			"{%.2f, %.2f, %.2f}",
			x, y, z
		);

		return std::string(out);
	}

	// raylib

	Vector3 raylib() const {
		return (Vector3){x, y, z};
	}

	void raylib(Vector3 v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	static Vec3 zero() {
		return Vec3(0.0f, 0.0f, 0.0f);
	}
	
	static Vec3 one() {
		return Vec3(1.0f, 1.0f, 1.0f);
	}
};

}
