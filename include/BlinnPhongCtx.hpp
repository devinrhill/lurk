#pragma once

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
