// Devin Hill 2026

#pragma once

#include <cstdio>
#include <raylib.h>
#include <cmath>
#include "../Math.hpp"

namespace lvk::util {

enum CurveType {
	C_NONE = 0,
    C_CONSTANT = 1,
    C_LINEAR = 2,

    C_EASE_IN_QUAD = 3,
    C_EASE_OUT_QUAD = 4,

    C_SINE_IN_OUT = 5,
    C_SINE_IN = 6,
    C_SINE_OUT = 7,

    C_COSINE_IN_OUT = 8,
    C_COSINE_OUT = 9,
    C_COSINE_IN = 10,

    C_COSINE_EASE_OUT = 11,
    C_SINE_EASE_IN = 12,
    C_SINE_EASE_OUT = 13,
    C_COSINE_EASE_IN = 14,

    C_SINE_IN_OUT_OVERSHOOT = 15,
    C_SINE_IN_OVERSHOOT = 16,
    C_SINE_OUT_OVERSHOOT = 17
};

float curve(int type, float start, float end, float rate) {
	//printf("%d %f %f %f %f\n", type, scalar, start, end, rate);
    if (start == end)
        return start;

    switch (type) {
    case CurveType::C_NONE:
    	return start;

    case CurveType::C_CONSTANT:
        rate = 0.0f;
        break;

    case CurveType::C_LINEAR:
        break;

    case CurveType::C_EASE_IN_QUAD:
        rate *= rate;
        break;

    case CurveType::C_EASE_OUT_QUAD:
        rate = 1.0f - rate * rate;
        break;

    case CurveType::C_SINE_IN_OUT:
        rate = (std::sin(rate * PI * 2.0f) + 1.0f) / 2.0f;
        break;

    case CurveType::C_SINE_IN:
        rate = std::sin(rate * PI);
        break;

    case CurveType::C_SINE_OUT:
        rate = std::sin(rate * PI + PI) + 1.0f;
        break;

    case CurveType::C_COSINE_IN_OUT:
        rate = (std::cos(rate * PI * 2.0f) + 1.0f) / 2.0f;
        break;

    case CurveType::C_COSINE_OUT:
        rate = (std::cos(rate * PI) + 1.0f) / 2.0f;
        break;

    case CurveType::C_COSINE_IN:
        rate = (std::cos(rate * PI + PI) + 1.0f) / 2.0f;
        break;

    case CurveType::C_COSINE_EASE_OUT:
        rate = std::cos(rate * PI / 2.0f);
        break;

    case CurveType::C_SINE_EASE_IN:
        rate = std::sin(rate * PI / 2.0f);
        break;

    case CurveType::C_SINE_EASE_OUT:
        rate = 1.0f - std::sin(rate * PI / 2.0f);
        break;

    case CurveType::C_COSINE_EASE_IN:
        rate = 1.0f - std::cos(rate * PI / 2.0f);
        break;

    case CurveType::C_SINE_IN_OUT_OVERSHOOT:
        rate = (2.0f * std::sin(rate * PI * 2.0f) + 1.0f) / 2.0f;
        rate = math::clamp<float>(rate, 0.0f, 1.0f);
        break;

    case CurveType::C_SINE_IN_OVERSHOOT:
        rate = 2.0f * std::sin(rate * PI);
        rate = std::min(rate, 1.0f);
        break;

    case CurveType::C_SINE_OUT_OVERSHOOT:
        rate = 2.0f * std::sin(rate * PI + PI) + 1.0f;
        rate = std::max(rate, 0.0f);
        break;
    }

    return (start + (end - start) * rate);
}

} // namespace lvk::util
