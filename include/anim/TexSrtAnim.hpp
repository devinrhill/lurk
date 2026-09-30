#pragma once

#include <raylib.h>
#include <raymath.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../io/BinaryIO.hpp"
#include "../io/BinaryReader.hpp"
#include "../io/Endianness.hpp"
#include "../util/Util.hpp"
#include "../util/Math.hpp"
#include "../GameSysCore.hpp"

#define TEX_SRT_ANIM_CAPACITY 128

enum TexSrtAnimTransformFlags {
	SRT_HAS_SCALE = 1,
	SRT_HAS_ROTATION = 2,
	SRT_HAS_TRANSLATION = 4,
	SRT_HAS_SHEAR = 8
};

struct TexSrtAnimKeyframe {
	float time;
	Vector2 scale;
	float rotationAngle;
	Vector2 translation;
	Vector2 shear;

	TexSrtAnimKeyframe* next;

	void init() {
		time = 0.0f;
		scale = Vector2One();
		rotationAngle = 0.0f;
		translation = Vector2Zero();
		shear = Vector2Zero();
		next = nullptr;
	}
};

struct TexSrtAnim {
	int keyframeCount;
	TexSrtAnimKeyframe* keyframes;

	float elapsedTime;
	float playbackSpeed;
	TexSrtAnimKeyframe* currentFrame;
	int currentFrameIndex;

	// interpolation state
	Vector2 currentScale;
	float currentRotationAngle;
	Vector2 currentTranslation;
	Vector2 currentShear;

	// shader state
	int scaleLoc;
	int rotationAngleLoc;
	int translationLoc;
	int shearLoc;

	void init() {
		keyframeCount = 0;
		keyframes = new TexSrtAnimKeyframe[TEX_SRT_ANIM_CAPACITY];
		for(int i = 0; i < TEX_SRT_ANIM_CAPACITY; i++) {
			keyframes[i].init();
		}
		elapsedTime = 0.0f;
		playbackSpeed = 1.0f;
		currentFrame = &keyframes[0];
		currentFrameIndex = 0;

		// init interpolation state
		currentScale = Vector2One();
		currentRotationAngle = 0.0f;
		currentTranslation = Vector2Zero();
		currentShear = Vector2Zero();

		// init shader state
		scaleLoc = -1;
		rotationAngleLoc = -1;
		translationLoc = -1;
		shearLoc = -1;

		getUniforms();
	}

	void close() {
		delete[] keyframes;
	}

	void update() {
		elapsedTime += GameCore.dt * playbackSpeed;

		TexSrtAnimKeyframe* curr = currentFrame;
		TexSrtAnimKeyframe* next = curr->next;

		// last frame
		if(next == nullptr) {
			currentFrame = &keyframes[0];
			currentFrameIndex = 0;
			elapsedTime = 0.0f;
			return;
		}

		// advance to next frame
		if(elapsedTime >= next->time) {
			currentFrame = next;
			currentFrameIndex = fwrapp(currentFrameIndex + 1, 0, keyframeCount-1);
			return;
		}

		// update current srt interpolated
		float dur = next->time - curr->time;
		if(dur <= 0.00001f) {
			return; // avoid divide by zero
		}

		float t = fclampp((elapsedTime - curr->time) / dur, 1.0f);

		// update state
		currentScale = Vector2Lerp(curr->scale, next->scale, t);	
		currentRotationAngle = Lerp(curr->rotationAngle, next->rotationAngle, t);	
		currentTranslation = Vector2Lerp(curr->translation, next->translation, t);	
		currentShear = Vector2Lerp(curr->shear, next->shear, t);	
	}

	void getUniforms() {
		scaleLoc = GetShaderLocation(GameCore.shctx.texSrtAnim, "scale");
		rotationAngleLoc = GetShaderLocation(GameCore.shctx.texSrtAnim, "rotationAngle");
		translationLoc = GetShaderLocation(GameCore.shctx.texSrtAnim, "translation");
		shearLoc = GetShaderLocation(GameCore.shctx.texSrtAnim, "shear");
	}

	void push() {
		SetShaderValue(GameCore.shctx.texSrtAnim, scaleLoc, &currentScale, SHADER_UNIFORM_VEC2);
		SetShaderValue(GameCore.shctx.texSrtAnim, rotationAngleLoc, &currentRotationAngle, SHADER_UNIFORM_FLOAT);
		SetShaderValue(GameCore.shctx.texSrtAnim, translationLoc, &currentTranslation, SHADER_UNIFORM_VEC2);
		SetShaderValue(GameCore.shctx.texSrtAnim, shearLoc, &currentShear, SHADER_UNIFORM_VEC2);
	}

	void load(const char* filename) {
		BinaryReader br;
		br.loadFile(filename);
		br.setEndianness(E_LITTLE);

		char magic[4];
		br.read(magic, 4, 1);
		if(memcmp(magic, "\x89SRT", 4)) {
			fprintf(stderr, "Fuck\n");
			return;
		}

		char endianness;
		br.read(&endianness, 1, 1);
		if(endianness == 0) {
			br.setEndianness(E_BIG);
		} else if(endianness == 1) {
			br.setEndianness(E_LITTLE);
		} else {
			printf("bad endiness\n");
			return;
		}

		char version;
		br.read(&version, 1, 1);
		if(version != 4) {
			printf("bad version\n");
			return;
		}

		unsigned int size = br.readInt<uint>().valueOr();
		if(size != br.getSize()) {
			fprintf(stderr, "Bad size\n");
			return;
		}

		br.seek(2, O_CUR); // skip reserved1

		keyframeCount = br.readInt<uint>().valueOr();
		if(keyframeCount > TEX_SRT_ANIM_CAPACITY) {
			fprintf(stderr, "Capacity\n");
			return;
		}

		unsigned int metadataOff = br.readInt<uint>().valueOr();
		unsigned int keyframesOff = br.readInt<uint>().valueOr();
		br.seek(0x8, O_CUR); // skip reserved2
		
		unsigned char* metadataFlags = new unsigned char[keyframeCount];
		for(int i = 0; i < keyframeCount; i++) {
			br.read(&metadataFlags[i], 1, 1);
		}

		while((br.tell() % 0x10) > 0) {
			br.seek(1, O_CUR);
		}

		for(int i = 0; i < keyframeCount; i++) {
			keyframes[i].time = br.readInt<float>().valueOr();
			if(metadataFlags[i] & SRT_HAS_SCALE) {
				keyframes[i].scale.x = br.readInt<float>().valueOr();
				keyframes[i].scale.y = br.readInt<float>().valueOr();
			}
			if(metadataFlags[i] & SRT_HAS_ROTATION) {
				keyframes[i].rotationAngle = br.readInt<float>().valueOr();
			}
			if(metadataFlags[i] & SRT_HAS_TRANSLATION) {
				keyframes[i].translation.x = br.readInt<float>().valueOr();
				keyframes[i].translation.y = br.readInt<float>().valueOr();
			}
			if(metadataFlags[i] & SRT_HAS_SHEAR) {
				keyframes[i].shear.x = br.readInt<float>().valueOr();
				keyframes[i].shear.y = br.readInt<float>().valueOr();
			}
		}

		for(int i = 0; i < keyframeCount-1; i++) {
			keyframes[i].next = &keyframes[i+1];
		}

		free(metadataFlags);

		return;
	}

	static TexSrtAnim create(const char* filename) {
		TexSrtAnim anim;

		anim.init();
		anim.load(filename);

		return anim;
	}
};
