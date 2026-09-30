#pragma once

#include <cstdio>
#include <cstring>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "../util/Curve.hpp"

struct Ptc3DGenConfig {
	enum Flags {
		PG_RAND_VEC_SET = 1<<0,
		PG_RAND_VEC_ADD = 1<<1,
		PG_RAND_VEC_MULT = 1<<2,
		PG_RAND_VEC_X = 1<<3,
		PG_RAND_VEC_Y = 1<<4,
		PG_RAND_VEC_Z = 1<<5,
		PG_RAND_VEC_XYZ = 
			PG_RAND_VEC_X | 
			PG_RAND_VEC_Y | 
			PG_RAND_VEC_Z,
		PG_RAND_UNIFORM = 1<<6,
		PG_RAND_SPHERE_DIST = 1<<7,
		PG_RAND_CUBE_DIST = 1<<8,
	};

	int ptcFlags;
	int burstCount;
	Vector3 pos;
	Vector3 scale;
	Vector3 vel;
	float speed;
	Vector3 accel;
	float mass;
	float lifetime;
	Color color;
	Matrix transform;
	Vector3 origin;

	bool isRandScale;
	int randScaleFlags;
	Vector2 randScaleRange;

	bool isRandVel;
	int randVelFlags;
	Vector2 randVelRange;

	bool isRandPos;
	int randPosFlags;
	Vector2 randPosRange;

	bool isRandOrigin;
	float randOriginInterval;
	int randOriginFlags;
	Vector2 randOriginRange;

	bool isRandRotation;
	Vector2 randRotationRange;

	int colorCurve;
	Color initColor;
	Color finalColor;

	char texPath[0x100];

	Ptc3DGenConfig() {
		ptcFlags = 0;
		burstCount = 0;
		pos = Vector3Zero();
		scale = Vector3One();
		vel = Vector3Zero();
		speed = 1.0f;
		accel = Vector3Zero();
		mass = 1.0f;
		lifetime = 0.0f;
		color = WHITE;
		transform = MatrixIdentity();
		origin = Vector3Zero();
		isRandScale = false;
		randScaleFlags = 0;
		randScaleRange = Vector2Zero();
		isRandVel = false;
		randVelFlags = 0;
		randVelRange = Vector2Zero();
		isRandPos = false;
		randPosFlags = 0;
		randPosRange = Vector2Zero();
		isRandOrigin = false;
		randOriginInterval = 0.0f;
		randOriginFlags = 0;
		randOriginRange = Vector2Zero();
		isRandRotation = false;
		randRotationRange = Vector2Zero();
		colorCurve = util::C_CONSTANT;
		initColor = WHITE;
		finalColor = WHITE;
		std::memset(texPath, 0, 0x100);
	}

	void print() {
		printf("========== PtcGenConfig3D ==========\n");

		printf("ptcFlags       = 0x%08X (%d)\n", ptcFlags, ptcFlags);

		printf("pos            = (%f, %f, %f)\n",
    		pos.x, pos.y, pos.z);

		printf("scale          = (%f, %f, %f)\n",
    		scale.x, scale.y, scale.z);

		printf("vel            = (%f, %f, %f)\n",
    		vel.x, vel.y, vel.z);

		printf("speed          = %f\n", speed);

		printf("accel          = (%f, %f, %f)\n",
    		accel.x, accel.y, accel.z);

		printf("lifetime       = %f\n", lifetime);

		printf("color          = (%d, %d, %d, %d)\n",
    		color.r, color.g, color.b, color.a);

		printf("transform:\n");
		printf("  m0 = %f %f %f %f\n",
    		transform.m0, transform.m1, transform.m2, transform.m3);
		printf("  m1 = %f %f %f %f\n",
    		transform.m4, transform.m5, transform.m6, transform.m7);
		printf("  m2 = %f %f %f %f\n",
    		transform.m8, transform.m9, transform.m10, transform.m11);
		printf("  m3 = %f %f %f %f\n",
    		transform.m12, transform.m13, transform.m14, transform.m15);

		printf("origin         = (%f, %f, %f)\n",
    		origin.x, origin.y, origin.z);

		printf("isRandScale    = %s\n",
    		isRandScale ? "true" : "false");

		printf("randScaleFlags = 0x%08X (%d)\n",
    		randScaleFlags, randScaleFlags);

		printf("randScaleRange = (%f, %f)\n",
    		randScaleRange.x, randScaleRange.y);

		printf("isRandVel      = %s\n",
    		isRandVel ? "true" : "false");

		printf("randVelFlags   = 0x%08X (%d)\n",
    		randVelFlags, randVelFlags);

		printf("randVelRange   = (%f, %f)\n",
    		randVelRange.x, randVelRange.y);

		printf("isRandPos      = %s\n",
    		isRandPos ? "true" : "false");

		printf("randPosFlags   = 0x%08X (%d)\n",
    		randPosFlags, randPosFlags);

		printf("randPosRange   = (%f, %f)\n",
    		randPosRange.x, randPosRange.y);

		/*
		printf("isColorLerp    = %s\n",
    		isColorLerp ? "true" : "false");
    	*/

		printf("initColor      = (%d, %d, %d, %d)\n",
    		initColor.r, initColor.g, initColor.b, initColor.a);

		printf("finalColor     = (%d, %d, %d, %d)\n",
    		finalColor.r, finalColor.g, finalColor.b, finalColor.a);

		printf("====================================\n");
	}
};

