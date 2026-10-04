#pragma once

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <vector>
#include "Ptc3D.hpp"
#include "Ptc3DGen.hpp"
#include "../GameSysCore.hpp"

struct Ptc3DEmi {
	struct Ptc3DSortBuffer {
		uint index;
		float distSq;
		float depth;
	};

	enum Flags {
		PE_CONTINUOUS = 1<<0,
		PE_BURST = 1<<1,
		PE_INITIAL = 1<<2
	};

	int ptcCount;
	int genCount;
	int flags;
	Ptc3D* ptcs;
	std::vector<Ptc3DGen*> gens;
	float spawnRate;
	float elapsed;
	float age;
	float delay;
	int sortCount;
	Ptc3DSortBuffer* sortBuffer;
	int highestNextFree;
	int lastHighestNextFree;
	bool doSort;

	void print() {
    	printf("PtcEmitter3D {\n");
    	printf("    ptcCount: %d\n", ptcCount);
    	printf("    genCount: %d\n", genCount);
    	printf("    flags: 0x%X\n", flags);
    	printf("    ptcs: %p\n", (void*)ptcs);
    	printf("    gens: %zu\n", gens.size());
    	printf("    spawnRate: %f\n", spawnRate);
    	printf("    elapsed: %f\n", elapsed);
    	printf("    age: %f\n", age);
    	printf("    delay: %f\n", delay);
    	printf("    sortCount: %d\n", sortCount);
    	printf("    sortBuffer: %p\n", (void*)sortBuffer);
    	printf("}\n");

    	for(uint i = 0; i < genCount; i++) {
    		gens[i]->print();
    	}
	}

	Ptc3DEmi() {
		highestNextFree = 0;
		ptcs = nullptr;
		sortBuffer = nullptr;
		flags = 0;
		spawnRate = 0.0f;
		age = 0.0f;
		delay = 0.0f;
		elapsed = 0.0f;
		sortCount = 0;
		doSort = true;
	}

	~Ptc3DEmi() {
		if(sortBuffer != nullptr) {
			delete[] sortBuffer;
			sortBuffer = nullptr;
		}

		if(ptcs != nullptr) {
			delete[] ptcs;
			ptcs = nullptr;
		}
	}

	void init() {
		sortBuffer = new Ptc3DSortBuffer[ptcCount];
		std::memset(sortBuffer, 0, sizeof(Ptc3DSortBuffer) * ptcCount);

		ptcs = new Ptc3D[ptcCount];
		std::memset(ptcs, 0, sizeof(Ptc3D) * ptcCount);
	}

	void gen() {
		for(int j = 0; j < genCount; j++) {
			uint count = (flags&PE_BURST && gens[j]->active)?gens[j]->config.burstCount:1;
			for(uint i = 0; i < count; i++) {
				if(gens[j]->active) {
					gens[j]->update();
					int nextFree = findNextFreePtc();

					lastHighestNextFree = highestNextFree;
					if(nextFree > highestNextFree) {
						highestNextFree = nextFree+1;
					}

					if(nextFree != -1) {
						elapsed = 0.0f;

						gens[j]->generate(&ptcs[nextFree]);
					}
				} else {
					break;
				}
			}

			gens[j]->post();
		}
	}

	void update() {
		if(IsKeyPressed(KEY_M)) {
			std::printf("particles used: %d\n", highestNextFree);
		}

		if(age == 0.0f && flags & PE_INITIAL) {
			gen();
		}

		if(isPastDelay()) {
			if(shouldSpawn()) {
				gen();
			}

			for(uint i = 0; i < ptcCount; i++) {
				ptcs[i].update();
			}

			elapsed += GetFrameTime();
		}

		age += GetFrameTime();
	}

	void draw() {
		sortCount = 0;

		Camera3D* cam = &GameCore.camera3d;

		for(uint i = 0; i < ptcCount; i++) {
			if(!ptcs[i].isAlive()) {
				continue;
			}

			if(doSort) {
				float halfWidth = ptcs[i].scale.x * 0.5f;

				Vector3 d = Vector3Subtract(ptcs[i].pos, cam->position);
				float depth = Vector3DotProduct(d, getCamera3DForward(*cam));

				float depthRadius = halfWidth;
				sortBuffer[sortCount++] = {
					i,
					Vector3LengthSqr(d),
					depth + depthRadius
				};
			}
		}

		if(doSort) {
			std::sort(sortBuffer,
				sortBuffer + sortCount,
				[](const Ptc3DSortBuffer& a, const Ptc3DSortBuffer& b) {
					return a.depth > b.depth;
				}
			);
		}

		rlEnableDepthTest();
		rlDisableDepthMask();
		BeginBlendMode(BLEND_ALPHA);
		if(doSort) {
			for(uint i = 0; i < sortCount; i++) {
				ptcs[sortBuffer[i].index].draw();
			}
		} else {
			for(uint i = 0; i < ptcCount; i++) {
				ptcs[i].draw();
			}
		}
		EndBlendMode();
		rlEnableDepthMask();
	}

	int findNextFreePtc() {
		for(uint i = 0; i < ptcCount; i++) {
			if(!ptcs[i].isAlive()) {
				return i;
			}
		}

		return -1;
	}

	bool shouldSpawn() {
		return elapsed > spawnRate;
	}

	bool isPastDelay() {
		return age > delay;
	}
};
