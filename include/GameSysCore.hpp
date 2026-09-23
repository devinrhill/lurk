#pragma once

#include <cmath>
#include <raylib.h>
#include "io/Endianness.hpp"
#include "util/Util.hpp"
#include "PhysicsCtx.hpp"
#include "ShaderCtx.hpp"
#include "WindowCtx.hpp"

struct GameSysCore {
	struct WindowCtx wctx;
	Camera3D camera3d;
	Camera2D camera2d;
	struct ShaderCtx shctx;
	struct PhysicsCtx pctx;

#ifndef __cplusplus
	int shouldRun;
#else
	bool shouldRun;
#endif
	float elapsedTime;
	int elapsedSeconds;
	float dt;
	int targetFps;
	int fps;
	uint64_t ticks;

	int endianness;

	void init() {
		endianness = getEndianness();
		g__zero = 0;
		g__one = 1;
		shctx.init();
		pctx.init();
	}

	void close() {
		shctx.close();
		pctx.close();
	}

	void update() {
		elapsedTime += GetFrameTime();
		elapsedSeconds = (int)floorf(elapsedTime);
		dt = GetFrameTime();
		ticks++;
	}
};

inline GameSysCore GameCore;
