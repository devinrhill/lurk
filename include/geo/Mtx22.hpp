// Devin Hill 2026

#pragma once

#include "Vec2.hpp"

namespace lvk::geo {

class Mtx22 {
public:
	float m[4] = {
		1.0f, 0.0f,
		0.0f, 1.0f
	};

	Mtx22() {}

	Mtx22(
		float m0, float m1,
		float m2, float m3
	) {
		m[0] = m0;
		m[1] = m1;
		m[2] = m2;
		m[3] = m3;
	}

	// indexing
	//
	// row/column indexing while internally using
	// the same column-major layout as raylib.

	float& operator()(int row, int column) {
		return m[column * 2 + row];
	}

	const float& operator()(int row, int column) const {
		return m[column * 2 + row];
	}

	float& operator[](int index) {
		return m[index];
	}

	const float& operator[](int index) const {
		return m[index];
	}

	// matrix arithmetic

	Mtx22 operator+(const Mtx22& other) const {
		Mtx22 out;

		for(int i = 0; i < 4; i++) {
			out.m[i] = m[i] + other.m[i];
		}

		return out;
	}

	Mtx22 operator-(const Mtx22& other) const {
		Mtx22 out;

		for(int i = 0; i < 4; i++) {
			out.m[i] = m[i] - other.m[i];
		}

		return out;
	}

	Mtx22 operator*(const Mtx22& other) const {
		Mtx22 out;

		for(int row = 0; row < 2; row++) {
			for(int column = 0; column < 2; column++) {
				out(row, column) =
					(*this)(row, 0) * other(0, column) +
					(*this)(row, 1) * other(1, column);
			}
		}

		return out;
	}

	// scalar arithmetic

	Mtx22 operator*(float scalar) const {
		Mtx22 out;

		for(int i = 0; i < 4; i++) {
			out.m[i] = m[i] * scalar;
		}

		return out;
	}

	Mtx22 operator/(float scalar) const {
		Mtx22 out;

		for(int i = 0; i < 4; i++) {
			out.m[i] = m[i] / scalar;
		}

		return out;
	}

	friend Mtx22 operator*(float scalar, const Mtx22& matrix) {
		return matrix * scalar;
	}

	// matrix-vector multiplication

	Vec2 operator*(const Vec2& v) const {
		return (Vec2){
			(*this)(0, 0) * v.x +
			(*this)(0, 1) * v.y,

			(*this)(1, 0) * v.x +
			(*this)(1, 1) * v.y
		};
	}

	// compound arithmetic

	Mtx22& operator+=(const Mtx22& other) {
		for(int i = 0; i < 4; i++) {
			m[i] += other.m[i];
		}

		return *this;
	}

	Mtx22& operator-=(const Mtx22& other) {
		for(int i = 0; i < 4; i++) {
			m[i] -= other.m[i];
		}

		return *this;
	}

	Mtx22& operator*=(const Mtx22& other) {
		*this = *this * other;

		return *this;
	}

	Mtx22& operator*=(float scalar) {
		for(int i = 0; i < 4; i++) {
			m[i] *= scalar;
		}

		return *this;
	}

	Mtx22& operator/=(float scalar) {
		for(int i = 0; i < 4; i++) {
			m[i] /= scalar;
		}

		return *this;
	}

	// comparison

	bool operator==(const Mtx22& other) const {
		for(int i = 0; i < 4; i++) {
			if(m[i] != other.m[i]) {
				return false;
			}
		}

		return true;
	}

	bool operator!=(const Mtx22& other) const {
		return !(*this == other);
	}

	// math

	Mtx22 transposed() const {
		Mtx22 out;

		for(int row = 0; row < 2; row++) {
			for(int column = 0; column < 2; column++) {
				out(row, column) = (*this)(column, row);
			}
		}

		return out;
	}

	void transpose() {
		*this = transposed();
	}

	float determinant() const {
		return (*this)(0, 0) * (*this)(1, 1) -
		       (*this)(0, 1) * (*this)(1, 0);
	}

	Mtx22 inverse() const {
		float det = determinant();

		if(det == 0.0f) {
			return Mtx22();
		}

		float invDet = 1.0f / det;

		return Mtx22(
			 (*this)(1, 1) * invDet,
			-(*this)(0, 1) * invDet,
			-(*this)(1, 0) * invDet,
			 (*this)(0, 0) * invDet
		);
	}

	void invert() {
		*this = inverse();
	}

	// common matrices

	static Mtx22 identity() {
		return Mtx22();
	}

	static Mtx22 zero() {
		return Mtx22(
			0.0f, 0.0f,
			0.0f, 0.0f
		);
	}
};

} // namespace lvk::geo
