// Devin Hill 2026

#pragma once

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "GeneratorConfig.hpp"
#include "../core/Math.hpp"
#include "../core/GameSysCore.hpp"
#include "../util/Raylib.hpp"

namespace lvk::ef3 {

struct Particle {
	enum Flags {
		PT_INTEGRATE = 1<<0,

		PT_DRAW_CUBE = 1<<1,
		PT_DRAW_QUAD = 1<<2,
		PT_DRAW_LOW_SPHERE = 1<<3,
		PT_DRAW_HIGH_SPHERE = 1<<4,
		PT_DRAW_BOUND_BOX = 1<<5,
		PT_DRAW_BILLBOARD_CYLINDER = 1<<6,
		PT_DRAW_BILLBOARD_SPHERE = 1<<7,

		PT_SHRINK_SCALE_AGE = 1<<8,
		PT_SH_BLACK_ALPHA = 1<<9
	};

	int flags;
	Vector3 pos;
	Vector3 scale;
	Vector3 initScale;
	Vector3 vel;
	Vector3 accel;
	float lifetime;
	float elapsedNorm;
	float elapsed;
	Color color;
	Color debugColor;
	Texture* texture;
	GeneratorConfig* genConfig;
	Vector3 force;
	float mass;
	float rotation;

	Particle() {
		flags = 0;
		pos = Vector3Zero();
		scale = Vector3One();
		initScale = scale;
		vel = Vector3Zero();
		accel = Vector3Zero();
		lifetime = 0.0f;
		elapsedNorm = 0.0f;
		elapsed = 0.0f;
		color = MAGENTA;
		debugColor = MAGENTA;
		//texture = nullptr;
		force = Vector3Zero();
		mass = 1.0f;
		rotation = 0.0f;
	}

	void update() {
		if(elapsed == 0.0f) {
			initScale = scale;
		}

		if(isAlive()) {
			if(flags & PT_INTEGRATE) {
				Vector3 useAccel = accel;
				useAccel = Vector3Scale(useAccel,mass*GameCore.dt);

				vel = Vector3Add(vel, Vector3Scale(useAccel, GameCore.dt));
				pos = Vector3Add(pos, Vector3Scale(vel, GameCore.dt));
			}

			if(flags & PT_SHRINK_SCALE_AGE) {
				if(elapsedNorm != 0.0f) {
					scale = Vector3Scale(initScale, 1.0f-elapsedNorm);
				}
			}

			if(genConfig->colorCurve != util::C_NONE) {
				color = util::curveColor(genConfig->colorCurve, genConfig->initColor, genConfig->finalColor, elapsedNorm);
			}

			elapsed += GameCore.dt;
			if(lifetime != 0.0f) {
				elapsedNorm = math::clamp(elapsed / lifetime, 0.0f, 1.0f);
			}
		}
	}

	void draw() {
		if(isAlive()) {
			if(flags & PT_DRAW_CUBE) {
				DrawCubeV(pos, scale, color);
			}
			if(flags & PT_DRAW_QUAD) {
				DrawPlane(pos, {scale.x, scale.z}, color);
			}
			if(flags & PT_DRAW_LOW_SPHERE) {
				DrawSphereEx(pos, Vector3Length(scale)/2.0f, 4, 6, color);
			}
			if(flags & PT_DRAW_HIGH_SPHERE) {
				DrawSphereEx(pos, Vector3Length(scale)/2.0f, 6, 8, color);
			}
			if(flags & PT_DRAW_BOUND_BOX) {
				DrawCubeWiresV(pos, Vector3Scale(scale, 2.0f), debugColor);
			}

			bool billboard = (flags & PT_DRAW_BILLBOARD_CYLINDER) || (flags & PT_DRAW_BILLBOARD_SPHERE);

			if(billboard) {
				bool shader = (flags & PT_SH_BLACK_ALPHA);

				if(flags & PT_SH_BLACK_ALPHA) {
					BeginShaderMode(GameCore.shctx.blackAlpha);
				}

				//rlSetBlendFactors(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD);

				Camera3D* camera = &GameCore.camera3d;

				Vector3 up = {0, 1, 0};
				if(flags & PT_DRAW_BILLBOARD_SPHERE) {
					Vector3 forward = Vector3Normalize(
    					Vector3Subtract(camera->position, pos)
					);

					Vector3 right = Vector3Normalize(
    					Vector3CrossProduct(camera->up, forward)
					);

					up = Vector3CrossProduct(forward, right);
				}

				DrawBillboardPro(
					GameCore.camera3d,
					*texture,
					(Rectangle){0, 0, (float)texture->width, (float)texture->height},
					pos,
					up,
					{scale.x, scale.z},
					{0, 0},
					rotation,
					color
				);

				if(shader) {
					EndShaderMode();
				}
			}
		}
	}

	bool isAlive() {
		return elapsed < lifetime;
	}
};

} // namespace lvk::ef3
