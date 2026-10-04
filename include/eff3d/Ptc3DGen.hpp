#pragma once

#include <cstdio>
#include <raylib.h>
#include <raymath.h>
#include "../util/Raylib.hpp"
#include "Ptc3D.hpp"
#include "Ptc3DGenConfig.hpp"

void randProc(Vector2 range, int flags, Vector3* dest, Ptc3DGenConfig conf) {
	Vector3 newv = Vector3Zero();
	if(flags & Ptc3DGenConfig::Flags::PG_RAND_SPHERE_DIST) {
		newv = Vector3Scale(
			getRandomSphereDir(),
			getRandomFloat(range.x, range.y)
		);
	} else if (flags & Ptc3DGenConfig::Flags::PG_RAND_CUBE_DIST) {
		newv = {
			getRandomFloat(range.x, range.y),
			getRandomFloat(range.x, range.y),
			getRandomFloat(range.x, range.y)
		};
	}

	if(flags & Ptc3DGenConfig::Flags::PG_RAND_UNIFORM) {
		newv.y = newv.x;
		newv.z = newv.x;
	}

	Vector3 modv = *dest;

	if(flags & Ptc3DGenConfig::Flags::PG_RAND_VEC_ADD) {
		modv = Vector3Add(modv, newv);
	} else if(flags & Ptc3DGenConfig::Flags::PG_RAND_VEC_SET) {
		modv = newv;
	} else if(flags & Ptc3DGenConfig::Flags::PG_RAND_VEC_MULT) {
		modv = Vector3Multiply(modv, newv);
	}

	if(flags & Ptc3DGenConfig::Flags::PG_RAND_VEC_X) {
		dest->x = modv.x;
	}
	if(flags & Ptc3DGenConfig::Flags::PG_RAND_VEC_Y) {
		dest->y = modv.y;
	}
	if(flags & Ptc3DGenConfig::Flags::PG_RAND_VEC_Z) {
		dest->z = modv.z;
	}
}

struct Ptc3DGen {
	Ptc3DGenConfig config;
	Texture texture;
	bool active;
	float randOriginElapsed;
	bool randOrigin;
	Vector3 initOrigin;

	Ptc3DGen() {
		//config = {};
		//texture = {};
		active = false;
		randOrigin = false;
		randOriginElapsed = 0.0f;
		initOrigin = Vector3Zero();
	}

	void print() {
    	printf("PtcGenerator3D {\n");
    	printf("    config: <Ptc3DGenConfig>\n");
    	printf("    texture: %p\n", (void*)&texture);
    	printf("    active: %s\n", active ? "true" : "false");
    	printf("}\n");
    	config.print();
	}

	void init() {
		printf("%s\n", config.texPath);
		texture = LoadTexture(config.texPath);
	}

	void update() {
		if(config.isRandOrigin) {
			randOriginElapsed += GetFrameTime();

			if(randOriginElapsed > config.randOriginInterval) {
				randOriginElapsed = 0.0f;

				randOrigin = true;
			}
		}
	}

	void generate(Ptc3D* ptc) {
		ptc->elapsed = ptc->elapsedNorm = 0.0f;
		ptc->pos = Vector3Add(
			config.origin,
			config.pos
		);
		ptc->scale = config.scale;
		ptc->vel = config.vel;
		ptc->accel = config.accel;
		ptc->mass = config.mass;
		ptc->lifetime = config.lifetime;
		ptc->color = config.color;
		ptc->debugColor = WHITE;
		ptc->flags = config.ptcFlags;
		ptc->texture = &texture;
		ptc->genConfig = &config;

		Vector2 range;
		int flags;
		Vector3* dest;
		if(config.isRandVel) {
			randProc(config.randVelRange, config.randVelFlags, &ptc->vel, config);
		}

		if(config.isRandScale) {
			randProc(config.randScaleRange, config.randScaleFlags, &ptc->scale, config);
		}

		if(config.isRandPos) {
			randProc(config.randPosRange, config.randPosFlags, &ptc->pos, config);
		}

		if(randOrigin) {
			randProc(config.randOriginRange, config.randOriginFlags, &config.origin, config);

			randOrigin = false;
		}

		ptc->vel = Vector3Scale(
			ptc->vel,
			config.speed
		);

		if(config.isRandRotation) {
			ptc->rotation = getRandomFloat(config.randRotationRange.x, config.randRotationRange.y);
		}
	}

	void post() {
		/*
		Vector2 range = {-1.0f, 1.0f};
		Vector3 dir = getRandomSphereDir();
		float rad = getRandomFloat(range.x, range.y);

		config.pos = Vector3Scale(
			dir,
			rad
		);

		config.vel = Vector3Scale(
			dir,
			rad
		);
		*/
	}
};

