// Devin Hill 2026

#pragma once

#include <cmath>
#include <raylib.h>
#include "../io/Endianness.hpp"
#include "../render/MultipassOverlay.hpp"
#include "PhysicsCtx.hpp"
#include "ShaderCtx.hpp"
#include "WindowCtx.hpp"

namespace lvk {

struct GameSysCore {
	struct WindowCtx wctx;
	Camera3D camera3d;
	Camera2D camera2d;
	struct ShaderCtx shctx;
	struct PhysicsCtx pctx;
	MultipassOverlay mpass;
	Color bgColor;
	int argc;
	char** argv;

#ifndef __cplusplus
	int shouldRun;
#else
	bool shouldRun;
#endif
	float elapsedTime;
	int elapsedSeconds;
	float dt;
	geo::Vec2 mousePosition;
	geo::Vec2 mouseDelta;
	int targetFps;
	int fps;
	uint64_t ticks;

	int endianness;

	void init() {
		endianness = io::getEndianness();
		shctx.init();
		pctx.init();
		bgColor = BLACK;
		mpass.init(wctx);
	}

	void close() {
		shctx.close();
		pctx.close();
		mpass.close();
	}

	void update() {
		elapsedTime += GetFrameTime();
		elapsedSeconds = (int)floorf(elapsedTime);
		dt = GetFrameTime();
		ticks++;
		mousePosition = GetMousePosition();
		mouseDelta = GetMouseDelta();
	}
};

inline GameSysCore GameCore;

} // namespace lvk
