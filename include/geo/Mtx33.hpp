// Devin Hill 2026

#pragma once

#include <cmath>
#include <cstdio>
#include <string>
#include "Vec3.hpp"

namespace lvk::geo {

class Mtx33 {
public:
	float m[9] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	Mtx33() {}

	~Mtx33() {}

	Mtx33(
		float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8
	) {
		m[0] = m0;
		m[1] = m1;
		m[2] = m2;
		m[3] = m3;
		m[4] = m4;
		m[5] = m5;
		m[6] = m6;
		m[7] = m7;
		m[8] = m8;
	}

	// indexing
	//
	// row/column indexing while internally using
	// column-major storage.

	float& operator()(int row, int column) {
		return m[column*3 + row];
	}

	const float& operator()(int row, int column) const {
		return m[column*3 + row];
	}

	float& operator[](int index) {
		return m[index];
	}

	const float& operator[](int index) const {
		return m[index];
	}

	// unary

	Mtx33 operator+() const {
		return *this;
	}

	Mtx33 operator-() const {
		return Mtx33(
			-m[0], -m[1], -m[2],
			-m[3], -m[4], -m[5],
			-m[6], -m[7], -m[8]
		);
	}

	// matrix arithmetic

	Mtx33 operator+(const Mtx33& other) const {
		Mtx33 out;

		for(int i = 0; i < 9; i++) {
			out.m[i] = m[i] + other.m[i];
		}

		return out;
	}

	Mtx33 operator-(const Mtx33& other) const {
		Mtx33 out;

		for(int i = 0; i < 9; i++) {
			out.m[i] = m[i] - other.m[i];
		}

		return out;
	}

	Mtx33 operator*(const Mtx33& other) const {
		Mtx33 out;

		for(int row = 0; row < 3; row++) {
			for(int column = 0; column < 3; column++) {
				out(row, column) =
					(*this)(row, 0) * other(0, column) +
					(*this)(row, 1) * other(1, column) +
					(*this)(row, 2) * other(2, column);
			}
		}

		return out;
	}

	// scalar arithmetic

	Mtx33 operator*(float scalar) const {
		Mtx33 out;

		for(int i = 0; i < 9; i++) {
			out.m[i] = m[i] * scalar;
		}

		return out;
	}

	Mtx33 operator/(float scalar) const {
		Mtx33 out;

		for(int i = 0; i < 9; i++) {
			out.m[i] = m[i] / scalar;
		}

		return out;
	}

	friend Mtx33 operator*(float scalar, const Mtx33& matrix) {
		return matrix * scalar;
	}

	// vector multiplication

	Vec3 operator*(const Vec3& v) const {
		return Vec3(
			(*this)(0, 0) * v.x +
			(*this)(0, 1) * v.y +
			(*this)(0, 2) * v.z,

			(*this)(1, 0) * v.x +
			(*this)(1, 1) * v.y +
			(*this)(1, 2) * v.z,

			(*this)(2, 0) * v.x +
			(*this)(2, 1) * v.y +
			(*this)(2, 2) * v.z
		);
	}

	// compound matrix arithmetic

	Mtx33& operator+=(const Mtx33& other) {
		for(int i = 0; i < 9; i++) {
			m[i] += other.m[i];
		}

		return *this;
	}

	Mtx33& operator-=(const Mtx33& other) {
		for(int i = 0; i < 9; i++) {
			m[i] -= other.m[i];
		}

		return *this;
	}

	Mtx33& operator*=(const Mtx33& other) {
		*this = *this * other;

		return *this;
	}

	// compound scalar arithmetic

	Mtx33& operator*=(float scalar) {
		for(int i = 0; i < 9; i++) {
			m[i] *= scalar;
		}

		return *this;
	}

	Mtx33& operator/=(float scalar) {
		for(int i = 0; i < 9; i++) {
			m[i] /= scalar;
		}

		return *this;
	}

	// comparison

	bool operator==(const Mtx33& other) const {
		for(int i = 0; i < 9; i++) {
			if(m[i] != other.m[i]) {
				return false;
			}
		}

		return true;
	}

	bool operator!=(const Mtx33& other) const {
		return !(*this == other);
	}

	// math

	float determinant() const {
		return
			m[0] * (m[4]*m[8] - m[7]*m[5]) -
			m[3] * (m[1]*m[8] - m[7]*m[2]) +
			m[6] * (m[1]*m[5] - m[4]*m[2]);
	}

	Mtx33 transposed() const {
		Mtx33 out;

		for(int row = 0; row < 3; row++) {
			for(int column = 0; column < 3; column++) {
				out(row, column) = (*this)(column, row);
			}
		}

		return out;
	}

	void transpose() {
		*this = transposed();
	}

	Mtx33 inverse() const {
		float det = determinant();

		if(det == 0.0f) {
			return Mtx33();
		}

		float invDet = 1.0f / det;

		return Mtx33(
			(m[4]*m[8] - m[7]*m[5]) * invDet,
			(m[7]*m[2] - m[1]*m[8]) * invDet,
			(m[1]*m[5] - m[4]*m[2]) * invDet,

			(m[6]*m[5] - m[3]*m[8]) * invDet,
			(m[0]*m[8] - m[6]*m[2]) * invDet,
			(m[3]*m[2] - m[0]*m[5]) * invDet,

			(m[3]*m[7] - m[6]*m[4]) * invDet,
			(m[6]*m[1] - m[0]*m[7]) * invDet,
			(m[0]*m[4] - m[3]*m[1]) * invDet
		);
	}

	void invert() {
		*this = inverse();
	}

	float trace() const {
		return m[0] + m[4] + m[8];
	}

	// common matrices

	static Mtx33 identity() {
		return Mtx33();
	}

	static Mtx33 zero() {
		return Mtx33(
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f
		);
	}

	static Mtx33 scale(float x, float y, float z) {
		return Mtx33(
			x,    0.0f, 0.0f,
			0.0f, y,    0.0f,
			0.0f, 0.0f, z
		);
	}

	static Mtx33 rotationX(float angle) {
		float c = std::cos(angle);
		float s = std::sin(angle);

		return Mtx33(
			1.0f, 0.0f, 0.0f,
			0.0f, c,    s,
			0.0f, -s,   c
		);
	}

	static Mtx33 rotationY(float angle) {
		float c = std::cos(angle);
		float s = std::sin(angle);

		return Mtx33(
			c,    0.0f, -s,
			0.0f, 1.0f, 0.0f,
			s,    0.0f, c
		);
	}

	static Mtx33 rotationZ(float angle) {
		float c = std::cos(angle);
		float s = std::sin(angle);

		return Mtx33(
			c,    s,    0.0f,
			-s,   c,    0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	// string

	std::string str() const {
		char out[0x200];

		std::snprintf(
			out,
			0x200,
			"{\n"
			"  {%.2f, %.2f, %.2f},\n"
			"  {%.2f, %.2f, %.2f},\n"
			"  {%.2f, %.2f, %.2f}\n"
			"}",
			(*this)(0, 0), (*this)(0, 1), (*this)(0, 2),
			(*this)(1, 0), (*this)(1, 1), (*this)(1, 2),
			(*this)(2, 0), (*this)(2, 1), (*this)(2, 2)
		);

		return std::string(out);
	}
};

} // namespace lvk::geo
