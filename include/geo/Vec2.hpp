#pragma once

#include <cmath>
#include <cstdio>
#include <raylib.h>
#include <string>

namespace geo {

class Vec2 {
public:
	float x = 0.0f;
	float y = 0.0f;

	Vec2() {}

	~Vec2() {}

	Vec2(float x, float y) : x{x}, y{y} {}

	Vec2(Vector2 v): x{v.x}, y{v.y} {}

	// unary

	Vec2 operator+() const {
		return Vec2(x, y);
	}

	Vec2 operator-() const {
		return Vec2(-x, -y);
	}

	// vector arithmetic

	Vec2 operator+(const Vec2& other) const {
		return Vec2(x+other.x, y+other.y);
	}

	Vec2 operator-(const Vec2& other) const {
		return Vec2(x-other.x, y-other.y);
	}

	Vec2 operator*(const Vec2& other) const {
		return Vec2(x*other.x, y*other.y);
	}

	Vec2 operator/(const Vec2& other) const {
		return Vec2(x/other.x, y/other.y);
	}

	// scalar arithmetic

	Vec2 operator*(float scalar) const {
		return Vec2(x*scalar, y*scalar);
	}

	Vec2 operator/(float scalar) const {
		return Vec2(x/scalar, y/scalar);
	}

	friend Vec2 operator*(float scalar, const Vec2& v) {
		return Vec2(v.x*scalar, v.y*scalar);
	}

	// compound vector arithmetic

	Vec2& operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;

		return *this;
	}

	Vec2& operator-=(const Vec2& other) {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	Vec2& operator*=(const Vec2& other) {
		x *= other.x;
		y *= other.y;

		return *this;
	}

	Vec2& operator/=(const Vec2& other) {
		x /= other.x;
		y /= other.y;

		return *this;
	}

	// compound scalar arithmetic

	Vec2& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vec2& operator/=(float scalar) {
		x /= scalar;
		y /= scalar;

		return *this;
	}

	// comparison

	bool operator==(const Vec2& other) const {
		return x == other.x &&
		       y == other.y;
	}

	bool operator!=(const Vec2& other) const {
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

	float dot(const Vec2& other) const {
		return x*other.x +
		       y*other.y;
	}

	float cross(const Vec2& other) const {
		return x*other.y -
		       y*other.x;
	}

	float lengthSqr() const {
		return x*x + y*y;
	}

	float length() const {
		return std::sqrt(lengthSqr());
	}

	Vec2 normalized() const {
		float len = length();

		if(len == 0.0f) {
			return Vec2();
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

	float distanceSqr(const Vec2& other) const {
		return (*this - other).lengthSqr();
	}

	float distance(const Vec2& other) const {
		return (*this - other).length();
	}

	// component helpers

	float min() const {
		return std::fmin(x, y);
	}

	float max() const {
		return std::fmax(x, y);
	}

	float sum() const {
		return x + y;
	}

	Vec2 abs() const {
		return Vec2(
			std::fabs(x),
			std::fabs(y)
		);
	}

	// string

	std::string str() const {
		char out[0x100];

		std::snprintf(
			out,
			0x100,
			"{%.2f, %.2f}",
			x, y
		);

		return std::string(out);
	}

	// raylib

	Vector2 raylib() const {
		return (Vector2){x, y};
	}

	void raylib(Vector2 v) {
		x = v.x;
		y = v.y;
	}

	static Vec2 zero() {
		return Vec2(0.0f, 0.0f);
	}
	
	static Vec2 one() {
		return Vec2(1.0f, 1.0f);
	}
};

}
