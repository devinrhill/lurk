// Devin Hill 2026

#pragma once

#include "../geo/Vec3.hpp"

using namespace lvk::geo;

namespace lvk {

struct BlinnPhongCtx {
	int lightPositionLoc;
	int lightColorLoc;
	int viewPositionLoc;
	int ambientStrengthLoc;
	int specularStrengthLoc;
	int shininessLoc;

	Vec3 lightPosition;
	Vec3 lightColor;
	float ambientStrength;
	float specularStrength;
	float shininess;
};

} // namespace lvk
