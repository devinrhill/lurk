#pragma once

#include <cmath>
#include <cstdio>
#include <raylib.h>
#include <string>
#include "Quat.hpp"
#include "Vec4.hpp"

namespace geo {

class Mtx44 {
public:
	float m[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	Mtx44() {}

	~Mtx44() {}

	Mtx44(
		float m0,  float m1,  float m2,  float m3,
		float m4,  float m5,  float m6,  float m7,
		float m8,  float m9,  float m10, float m11,
		float m12, float m13, float m14, float m15
	) {
		m[0]  = m0;
		m[1]  = m1;
		m[2]  = m2;
		m[3]  = m3;
		m[4]  = m4;
		m[5]  = m5;
		m[6]  = m6;
		m[7]  = m7;
		m[8]  = m8;
		m[9]  = m9;
		m[10] = m10;
		m[11] = m11;
		m[12] = m12;
		m[13] = m13;
		m[14] = m14;
		m[15] = m15;
	}

	Mtx44(Matrix other) {
		m[0] = other.m0;
		m[1] = other.m1;
		m[2] = other.m2;
		m[3] = other.m3;
		m[4] = other.m4;
		m[5] = other.m5;
		m[6] = other.m6;
		m[7] = other.m7;
		m[8] = other.m8;
		m[9] = other.m9;
		m[10] = other.m10;
		m[11] = other.m11;
		m[12] = other.m12;
		m[13] = other.m13;
		m[14] = other.m14;
		m[15] = other.m15;
	}

	// indexing
	//
	// row/column indexing while internally using
	// the same column-major layout as raylib.

	float& operator()(int row, int column) {
		return m[column*4 + row];
	}

	const float& operator()(int row, int column) const {
		return m[column*4 + row];
	}

	float& operator[](int index) {
		return m[index];
	}

	const float& operator[](int index) const {
		return m[index];
	}

	// unary

	Mtx44 operator+() const {
		return *this;
	}

	Mtx44 operator-() const {
		return Mtx44(
			-m[0],  -m[1],  -m[2],  -m[3],
			-m[4],  -m[5],  -m[6],  -m[7],
			-m[8],  -m[9],  -m[10], -m[11],
			-m[12], -m[13], -m[14], -m[15]
		);
	}

	// matrix arithmetic

	Mtx44 operator+(const Mtx44& other) const {
		Mtx44 out;

		for(int i = 0; i < 16; i++) {
			out.m[i] = m[i] + other.m[i];
		}

		return out;
	}

	Mtx44 operator-(const Mtx44& other) const {
		Mtx44 out;

		for(int i = 0; i < 16; i++) {
			out.m[i] = m[i] - other.m[i];
		}

		return out;
	}

	Mtx44 operator*(const Mtx44& other) const {
		Mtx44 out;

		for(int row = 0; row < 4; row++) {
			for(int column = 0; column < 4; column++) {
				out(row, column) =
					(*this)(row, 0) * other(0, column) +
					(*this)(row, 1) * other(1, column) +
					(*this)(row, 2) * other(2, column) +
					(*this)(row, 3) * other(3, column);
			}
		}

		return out;
	}

	// scalar arithmetic

	Mtx44 operator*(float scalar) const {
		Mtx44 out;

		for(int i = 0; i < 16; i++) {
			out.m[i] = m[i] * scalar;
		}

		return out;
	}

	Mtx44 operator/(float scalar) const {
		Mtx44 out;

		for(int i = 0; i < 16; i++) {
			out.m[i] = m[i] / scalar;
		}

		return out;
	}

	friend Mtx44 operator*(float scalar, const Mtx44& matrix) {
		return matrix * scalar;
	}

	// matrix-vector multiplication

	Vec4 operator*(const Vec4& v) const {
		return (Vec4){
			(*this)(0, 0) * v.x +
			(*this)(0, 1) * v.y +
			(*this)(0, 2) * v.z +
			(*this)(0, 3) * v.w,

			(*this)(1, 0) * v.x +
			(*this)(1, 1) * v.y +
			(*this)(1, 2) * v.z +
			(*this)(1, 3) * v.w,

			(*this)(2, 0) * v.x +
			(*this)(2, 1) * v.y +
			(*this)(2, 2) * v.z +
			(*this)(2, 3) * v.w,

			(*this)(3, 0) * v.x +
			(*this)(3, 1) * v.y +
			(*this)(3, 2) * v.z +
			(*this)(3, 3) * v.w
		};
	}

	// compound matrix arithmetic

	Mtx44& operator+=(const Mtx44& other) {
		for(int i = 0; i < 16; i++) {
			m[i] += other.m[i];
		}

		return *this;
	}

	Mtx44& operator-=(const Mtx44& other) {
		for(int i = 0; i < 16; i++) {
			m[i] -= other.m[i];
		}

		return *this;
	}

	Mtx44& operator*=(const Mtx44& other) {
		*this = *this * other;

		return *this;
	}

	// compound scalar arithmetic

	Mtx44& operator*=(float scalar) {
		for(int i = 0; i < 16; i++) {
			m[i] *= scalar;
		}

		return *this;
	}

	Mtx44& operator/=(float scalar) {
		for(int i = 0; i < 16; i++) {
			m[i] /= scalar;
		}

		return *this;
	}

	// comparison

	bool operator==(const Mtx44& other) const {
		for(int i = 0; i < 16; i++) {
			if(m[i] != other.m[i]) {
				return false;
			}
		}

		return true;
	}

	bool operator!=(const Mtx44& other) const {
		return !(*this == other);
	}

	// math

	Mtx44 transposed() const {
		Mtx44 out;

		for(int row = 0; row < 4; row++) {
			for(int column = 0; column < 4; column++) {
				out(row, column) = (*this)(column, row);
			}
		}

		return out;
	}

	void transpose() {
		*this = transposed();
	}

	float determinant() const {
		float a = m[0];
		float b = m[4];
		float c = m[8];
		float d = m[12];

		float e = m[1];
		float f = m[5];
		float g = m[9];
		float h = m[13];

		float i = m[2];
		float j = m[6];
		float k = m[10];
		float l = m[14];

		float mm = m[3];
		float n = m[7];
		float o = m[11];
		float p = m[15];

		return
			a * (
				f * (k*p - l*o) -
				g * (j*p - l*n) +
				h * (j*o - k*n)
			)
			- b * (
				e * (k*p - l*o) -
				g * (i*p - l*mm) +
				h * (i*o - k*mm)
			)
			+ c * (
				e * (j*p - l*n) -
				f * (i*p - l*mm) +
				h * (i*n - j*mm)
			)
			- d * (
				e * (j*o - k*n) -
				f * (i*o - k*mm) +
				g * (i*n - j*mm)
			);
	}

	Mtx44 inverse() const {
		float det = determinant();

		if(det == 0.0f) {
			return Mtx44();
		}

		Mtx44 out;

		float invDet = 1.0f / det;

		out.m[0] =
			(m[5] * (m[10]*m[15] - m[14]*m[11]) -
			 m[9] * (m[6]*m[15] - m[14]*m[7]) +
			 m[13] * (m[6]*m[11] - m[10]*m[7])) * invDet;

		out.m[4] =
			-(m[4] * (m[10]*m[15] - m[14]*m[11]) -
			  m[8] * (m[6]*m[15] - m[14]*m[7]) +
			  m[12] * (m[6]*m[11] - m[10]*m[7])) * invDet;

		out.m[8] =
			(m[4] * (m[9]*m[15] - m[13]*m[11]) -
			 m[8] * (m[5]*m[15] - m[13]*m[7]) +
			 m[12] * (m[5]*m[11] - m[9]*m[7])) * invDet;

		out.m[12] =
			-(m[4] * (m[9]*m[14] - m[13]*m[10]) -
			  m[8] * (m[5]*m[14] - m[13]*m[6]) +
			  m[12] * (m[5]*m[10] - m[9]*m[6])) * invDet;

		out.m[1] =
			-(m[1] * (m[10]*m[15] - m[14]*m[11]) -
			  m[9] * (m[2]*m[15] - m[14]*m[3]) +
			  m[13] * (m[2]*m[11] - m[10]*m[3])) * invDet;

		out.m[5] =
			(m[0] * (m[10]*m[15] - m[14]*m[11]) -
			 m[8] * (m[2]*m[15] - m[14]*m[3]) +
			 m[12] * (m[2]*m[11] - m[10]*m[3])) * invDet;

		out.m[9] =
			-(m[0] * (m[9]*m[15] - m[13]*m[11]) -
			  m[8] * (m[1]*m[15] - m[13]*m[3]) +
			  m[12] * (m[1]*m[11] - m[9]*m[3])) * invDet;

		out.m[13] =
			(m[0] * (m[9]*m[14] - m[13]*m[10]) -
			 m[8] * (m[1]*m[14] - m[13]*m[2]) +
			 m[12] * (m[1]*m[10] - m[9]*m[2])) * invDet;

		out.m[2] =
			(m[1] * (m[6]*m[15] - m[14]*m[7]) -
			 m[5] * (m[2]*m[15] - m[14]*m[3]) +
			 m[13] * (m[2]*m[7] - m[6]*m[3])) * invDet;

		out.m[6] =
			-(m[0] * (m[6]*m[15] - m[14]*m[7]) -
			  m[4] * (m[2]*m[15] - m[14]*m[3]) +
			  m[12] * (m[2]*m[7] - m[6]*m[3])) * invDet;

		out.m[10] =
			(m[0] * (m[5]*m[15] - m[13]*m[7]) -
			 m[4] * (m[1]*m[15] - m[13]*m[3]) +
			 m[12] * (m[1]*m[7] - m[5]*m[3])) * invDet;

		out.m[14] =
			-(m[0] * (m[5]*m[14] - m[13]*m[6]) -
			  m[4] * (m[1]*m[14] - m[13]*m[2]) +
			  m[12] * (m[1]*m[6] - m[5]*m[2])) * invDet;

		out.m[3] =
			-(m[1] * (m[6]*m[11] - m[10]*m[7]) -
			  m[5] * (m[2]*m[11] - m[10]*m[3]) +
			  m[9] * (m[2]*m[7] - m[6]*m[3])) * invDet;

		out.m[7] =
			(m[0] * (m[6]*m[11] - m[10]*m[7]) -
			 m[4] * (m[2]*m[11] - m[10]*m[3]) +
			 m[8] * (m[2]*m[7] - m[6]*m[3])) * invDet;

		out.m[11] =
			-(m[0] * (m[5]*m[11] - m[9]*m[7]) -
			  m[4] * (m[1]*m[11] - m[9]*m[3]) +
			  m[8] * (m[1]*m[7] - m[5]*m[3])) * invDet;

		out.m[15] =
			(m[0] * (m[5]*m[10] - m[9]*m[6]) -
			 m[4] * (m[1]*m[10] - m[9]*m[2]) +
			 m[8] * (m[1]*m[6] - m[5]*m[2])) * invDet;

		return out;
	}

	void invert() {
		*this = inverse();
	}

	// common matrices

	static Mtx44 identity() {
		return Mtx44();
	}

	static Mtx44 zero() {
		return Mtx44(
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		);
	}

	static Mtx44 translation(float x, float y, float z) {
		return Mtx44(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x,    y,    z,    1.0f
		);
	}

	static Mtx44 scale(float x, float y, float z) {
		return Mtx44(
			x,    0.0f, 0.0f, 0.0f,
			0.0f, y,    0.0f, 0.0f,
			0.0f, 0.0f, z,    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static Mtx44 rotationX(float angle) {
		float c = std::cos(angle);
		float s = std::sin(angle);

		return Mtx44(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, c,    s,    0.0f,
			0.0f, -s,   c,    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static Mtx44 rotationY(float angle) {
		float c = std::cos(angle);
		float s = std::sin(angle);

		return Mtx44(
			c,    0.0f, -s,   0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			s,    0.0f, c,    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	static Mtx44 rotationZ(float angle) {
		float c = std::cos(angle);
		float s = std::sin(angle);

		return Mtx44(
			c,    s,    0.0f, 0.0f,
			-s,   c,    0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	// string

	std::string str() const {
		char out[0x400];

		std::snprintf(
			out,
			0x400,
			"{\n"
			"  {%.2f, %.2f, %.2f, %.2f},\n"
			"  {%.2f, %.2f, %.2f, %.2f},\n"
			"  {%.2f, %.2f, %.2f, %.2f},\n"
			"  {%.2f, %.2f, %.2f, %.2f}\n"
			"}",
			(*this)(0, 0), (*this)(0, 1), (*this)(0, 2), (*this)(0, 3),
			(*this)(1, 0), (*this)(1, 1), (*this)(1, 2), (*this)(1, 3),
			(*this)(2, 0), (*this)(2, 1), (*this)(2, 2), (*this)(2, 3),
			(*this)(3, 0), (*this)(3, 1), (*this)(3, 2), (*this)(3, 3)
		);

		return std::string(out);
	}

	// raylib

	Matrix raylib() const {
		return (Matrix){
			m[0],  m[1],  m[2],  m[3],
			m[4],  m[5],  m[6],  m[7],
			m[8],  m[9],  m[10], m[11],
			m[12], m[13], m[14], m[15]
		};
	}

	void raylib(Matrix v) {
		m[0]  = v.m0;
		m[1]  = v.m1;
		m[2]  = v.m2;
		m[3]  = v.m3;
		m[4]  = v.m4;
		m[5]  = v.m5;
		m[6]  = v.m6;
		m[7]  = v.m7;
		m[8]  = v.m8;
		m[9]  = v.m9;
		m[10] = v.m10;
		m[11] = v.m11;
		m[12] = v.m12;
		m[13] = v.m13;
		m[14] = v.m14;
		m[15] = v.m15;
	}

	static Mtx44 rotation(const Quat& q) {
		float xx = q.x * q.x;
		float yy = q.y * q.y;
		float zz = q.z * q.z;

		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yz = q.y * q.z;

		float wx = q.w * q.x;
		float wy = q.w * q.y;
		float wz = q.w * q.z;

		return Mtx44(
			1.0f - 2.0f * (yy + zz),
			2.0f * (xy - wz),
			2.0f * (xz + wy),
			0.0f,

			2.0f * (xy + wz),
			1.0f - 2.0f * (xx + zz),
			2.0f * (yz - wx),
			0.0f,

			2.0f * (xz - wy),
			2.0f * (yz + wx),
			1.0f - 2.0f * (xx + yy),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
	}
};

}
