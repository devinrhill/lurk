#pragma once

#include <raylib.h>
#include <cmath>
#include "Math.hpp"

namespace util {

enum CurveType {
    C_CONSTANT = 0,
    C_LINEAR = 1,

    C_EASE_IN_QUAD = 2,
    C_EASE_OUT_QUAD = 3,

    C_SINE_IN_OUT = 4,
    C_SINE_IN = 5,
    C_SINE_OUT = 6,

    C_COSINE_IN_OUT = 7,
    C_COSINE_OUT = 8,
    C_COSINE_IN = 9,

    C_COSINE_EASE_OUT = 10,
    C_SINE_EASE_IN = 11,
    C_SINE_EASE_OUT = 12,
    C_COSINE_EASE_IN = 13,

    C_SINE_IN_OUT_OVERSHOOT = 14,
    C_SINE_IN_OVERSHOOT = 15,
    C_SINE_OUT_OVERSHOOT = 16
};

float curve(int type, float start, float end, float rate) {
    if (start == end)
        return start;

    switch (type) {
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
        rate = util::fclamp(rate, 0.0f, 1.0f);
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

    return start + (end - start) * rate;
}

}
