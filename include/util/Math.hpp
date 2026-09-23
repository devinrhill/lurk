#pragma once

#include <math.h>

double min(double x, double y) {
	if(x < y) {
		return x;
	} else {
		return y;
	}
}

double max(double x, double y) {
	if(x > y) {
		return x;
	} else {
		return y;
	}
}

float pfclampp(float* x, float max) {
	if(*x > max) {
		*x = max;
	}

	return *x;
}

float pfclampn(float* x, float min) {
	if(*x < min) {
		*x = min;
	}

	return *x;
}

float pfclamp(float* x, float min, float max) {
	if(*x < min) {
		*x = min;
	} else if(*x > max) {
		*x = max;
	}

	return *x;
}

float fclampp(float x, float max) {
	return (x > max) ? max : x;
}

float fclampn(float x, float min) {
	return (x < min) ? min : x;
}

float fclamp(float x, float min, float max) {
	if(x < min) {
		return min;
	} else if(x > max) {
		return max;
	}

	return x;
}

float pfwrapp(float* x, float min, float max) {
	if(*x > max) {
		*x = min;
	}

	return *x;
}

float pfwrapn(float* x, float min, float max) {
	if(*x < min) {
		*x = max;
	}

	return *x;
}

float pfwrap(float* x, float min, float max) {
	if(*x < min) {
		*x = max;
	} else if(*x > max) {
		*x = min;
	}

	return *x;
}

float fwrapp(float x, float min, float max) {
	return (x > max) ? min : x;
}

float fwrapn(float x, float min, float max) {
	return (x < min) ? max : x;
}

float fwrap(float x, float min, float max) {
	if(x < min) {
		return max;
	} else if(x > max) {
		return min;
	}

	return x;
}

int feqe(float x, float y, float eps) {
	return (fabs(x-y) < eps);
}

int deqe(double x, double y, float eps) {
	return (fabs(x-y) < eps);
}

int feqel(float x, float y) {
	return feqe(x, y, 0.001f);
}

int feqeh(float x, float y) {
	return feqe(x, y, 0.000001f);
}

int deqel(double x, double y) {
	return deqe(x, y, 0.001);
}

int deqeh(double x, double y) {
	return deqe(x, y, 0.000001);
}
