// Devin Hill 2026

#pragma once

#include <cstdio>
#include <vector>
#include "../io/BinaryReader.hpp"
#include "../io/Endianness.hpp"
#include "Emitter.hpp"
#include "../task/Task.hpp"

using namespace lvk::io;

namespace lvk::ef3 {

struct TaskEffect3D: public Task {
	// 1
	static constexpr uint VERSION = 2;

	std::vector<Emitter*> emitters;

	TaskEffect3D() {
		setName("TaskEffect3D");
		flags |= UPDATE | DRAW;
		drawFlags[MAIN] |= DRAW_3D;
	}

	bool update(Task* param) override {
		for(Emitter* e: emitters) {
			e->update();
		}

		return true;
	}

	void draw(int status, Task* param) override {
		for(Emitter* e: emitters) {
			e->draw();
		}
	}

	static TaskEffect3D create(const char* filename) {
		TaskEffect3D eff;

		eff.loadFile(filename);

		return eff;
	}

	void clear() {
		emitters.clear();
	}

	void loadFile(const char* filename) {
		BinaryReader br;

		br.loadFile(filename);

		char magic[4];
		br.read(magic, 1, 4);
		if(memcmp(magic, "\x89PTC", 4)) {
			std::printf("magic %s\n", magic);
			return;
		}

		uchar endianness = br.readInt<uchar>().valueOr();
		if(endianness == 0) {
			br.setEndianness(E_BIG);
		} else if(endianness == 1) {
			br.setEndianness(E_LITTLE);
		} else {
			return;
		}

		uchar version = br.readInt<uchar>().valueOr();
		if(version != VERSION) {
			return;
		}

		/*
		std::size_t size = br.readInt<std::size_t>().valueOr();
		if(size != br.getSize()) {
			return;
		}
		*/

		br.seek(6, O_CUR);

		uint emitterCount = br.readInt<uint>().valueOr();

		for(uint i = 0; i < emitterCount; i++) {
			Emitter* emi = new Emitter;

			char emiMagic[4];
			br.read(emiMagic, 4, 1);


			emi->ptcCount = br.readInt<uint>().valueOr();

			emi->flags = br.readInt<uint>().valueOr();
			emi->delay = br.readInt<float>().valueOr();
			emi->spawnRate = br.readInt<float>().valueOr();
			emi->genCount = br.readInt<uint>().valueOr();

			emi->init();

			br.seek(8, O_CUR);

			for(uint j = 0; j < emi->genCount; j++) {
				Generator* gen = new Generator;

				char genMagic[4];
				br.read(genMagic, 1, 4);

				gen->config.ptcFlags = br.readInt<uint>().valueOr();

				gen->config.burstCount = br.readInt<uint>().valueOr();


				gen->config.pos.x = br.readInt<float>().valueOr();
				gen->config.pos.y = br.readInt<float>().valueOr();
				gen->config.pos.z = br.readInt<float>().valueOr();


				gen->config.scale.x = br.readInt<float>().valueOr();
				gen->config.scale.y = br.readInt<float>().valueOr();
				gen->config.scale.z = br.readInt<float>().valueOr();


				gen->config.vel.x = br.readInt<float>().valueOr();
				gen->config.vel.y = br.readInt<float>().valueOr();
				gen->config.vel.z = br.readInt<float>().valueOr();


				gen->config.speed = br.readInt<float>().valueOr();

				gen->config.accel.x = br.readInt<float>().valueOr();
				gen->config.accel.y = br.readInt<float>().valueOr();
				gen->config.accel.z = br.readInt<float>().valueOr();

				gen->config.mass = br.readInt<float>().valueOr();


				gen->config.lifetime = br.readInt<float>().valueOr();

				gen->config.color.r = br.readInt<uchar>().valueOr();
				gen->config.color.g = br.readInt<uchar>().valueOr();
				gen->config.color.b = br.readInt<uchar>().valueOr();
				gen->config.color.a = br.readInt<uchar>().valueOr();

				gen->config.origin.x = br.readInt<float>().valueOr();
				gen->config.origin.y = br.readInt<float>().valueOr();
				gen->config.origin.z = br.readInt<float>().valueOr();

				br.seek(0x40, O_CUR);


				gen->config.isRandScale = br.readInt<uint>().valueOr();
				gen->config.randScaleRange.x = br.readInt<float>().valueOr();
				gen->config.randScaleRange.y = br.readInt<float>().valueOr();
				gen->config.randScaleFlags = br.readInt<uint>().valueOr();



				gen->config.isRandVel = br.readInt<uint>().valueOr();
				gen->config.randVelRange.x = br.readInt<float>().valueOr();
				gen->config.randVelRange.y = br.readInt<float>().valueOr();
				gen->config.randVelFlags = br.readInt<uint>().valueOr();


				gen->config.isRandPos = br.readInt<uint>().valueOr();
				gen->config.randPosRange.x = br.readInt<float>().valueOr();
				gen->config.randPosRange.y = br.readInt<float>().valueOr();
				gen->config.randPosFlags = br.readInt<uint>().valueOr();

				gen->config.isRandOrigin = br.readInt<uint>().valueOr();
				gen->config.randOriginInterval = br.readInt<float>().valueOr();
				gen->config.randOriginFlags = br.readInt<uint>().valueOr();
				gen->config.randOriginRange.x = br.readInt<float>().valueOr();
				gen->config.randOriginRange.y = br.readInt<float>().valueOr();


				gen->config.isRandRotation = br.readInt<uint>().valueOr();
				gen->config.randRotationRange.x = br.readInt<float>().valueOr();
				gen->config.randRotationRange.y = br.readInt<float>().valueOr();

				gen->config.color.r = br.readInt<uchar>().valueOr();
				gen->config.color.g = br.readInt<uchar>().valueOr();
				gen->config.color.b = br.readInt<uchar>().valueOr();
				gen->config.color.a = br.readInt<uchar>().valueOr();

				gen->config.colorCurve = br.readInt<uint>().valueOr();
				gen->config.colorCurveScalar = br.readInt<float>().valueOr();
				printf("scalar %f\n", gen->config.colorCurveScalar);

				gen->config.initColor.r = br.readInt<uchar>().valueOr();
				gen->config.initColor.g = br.readInt<uchar>().valueOr();
				gen->config.initColor.b = br.readInt<uchar>().valueOr();
				gen->config.initColor.a = br.readInt<uchar>().valueOr();

				gen->config.finalColor.r = br.readInt<uchar>().valueOr();
				gen->config.finalColor.g = br.readInt<uchar>().valueOr();
				gen->config.finalColor.b = br.readInt<uchar>().valueOr();
				gen->config.finalColor.a = br.readInt<uchar>().valueOr();

				br.readLStr((char*)gen->config.texPath);

				gen->active = true;
				gen->init();

				//br.seek(4, O_CUR);


				emi->gens.push_back(gen);
			}

			emitters.push_back(emi);
		}

	}

	void print() {

	}
};

} // namespace lvk::ef3
