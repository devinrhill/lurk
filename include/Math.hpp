// Devin Hill 2026

#pragma once

namespace lvk::math {

template<typename T>
T abs(T x) {
	if(x < (T)0) {
		return -x;
	}

	return x;
}

template<typename T>
bool eqe(T x, T y, T eps) {
	return (abs<T>(x-y) < eps);
}

template<typename T>
bool eqel(T x, T y) {
	return eqe<T>(x, y, 0.001f);
}

template<typename T>
bool eqeh(T x, T y) {
	return eqe<T>(x, y, 0.000001f);
}

template<typename T>
inline T min(T x, T y) {
	if(x < y) {
		return x;
	} else {
		return y;
	}
}

template<typename T>
inline T max(T x, T y) {
	if(x > y) {
		return x;
	} else {
		return y;
	}
}

template<typename T>
T clamp(T x, T min, T max) {
	if(x < min) {
		return min;
	} else if(x > max) {
		return max;
	}

	return x;
}

template<typename T>
T warp(T x, T min, T max) {
	if(x < min) {
		return max;
	} else if(x > max) {
		return min;
	}

	return x;
}

template<typename T>
T wrap(T x, T min, T max) {
	if(x < min) {
		return min; // TODO: fix wtf
	} else if(x > max) {
		return x % max;
	}

	return x;
}

} // namespace lvk::math
