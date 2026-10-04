// Devin Hill 2026

#pragma once

namespace lvk {

struct PhysicsCtx {
	int fps;
	double dt;
	double accumulator;
	double step;
	double time;

	void init() {
		fps = 60;
		dt = 0.0;
		accumulator = 0.0;
		step = 1.0 / (double)fps;
		time = 0.0;
	}

	void close() {

	}
};

#define PHYS_BEGIN(pctx) \
	pctx.dt = GameCore.dt; \
	if(pctx.dt > 0.25) { \
		pctx.dt = 0.25; \
	} \
	pctx.accumulator += pctx.dt; \
	while(pctx.accumulator >= pctx.step) {

#define PHYS_END(pctx) \
		pctx.time += pctx.step; \
		pctx.accumulator -= pctx.step; \
	}

} // namespace lvk
