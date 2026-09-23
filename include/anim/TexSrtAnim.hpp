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

	struct TexSrtAnimKeyframe* next;

	TexSrtAnimKeyframe() {
		time = 0.0f;
		scale = Vector2One();
		rotationAngle = 0.0f;
		translation = Vector2Zero();
		next = NULL;
	}
};

struct TexSrtAnim {
	int keyframeCount;
	struct TexSrtAnimKeyframe* keyframes;

	float elapsedTime;
	float fps;
	struct TexSrtAnimKeyframe* currentFrame;
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

	TexSrtAnim() {
		keyframeCount = 0;
		keyframes = (TexSrtAnimKeyframe*)zalloc(sizeof(struct TexSrtAnimKeyframe) * TEX_SRT_ANIM_CAPACITY);
		elapsedTime = 0.0f;
		fps = 60.0f;
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

	~TexSrtAnim() {
		if(keyframes) {
			free(keyframes);
			keyframes = NULL;
		}
		keyframeCount = 0;
		currentFrame = NULL;
	}

	void update() {
		elapsedTime += GetFrameTime() * fps;

		struct TexSrtAnimKeyframe* curr = currentFrame;
		struct TexSrtAnimKeyframe* next = curr->next;

		// last frame
		if(next == NULL) {
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
			return;
		}

		char version;
		br.read(&version, 1, 1);
		if(version != 4) {
			return;
		}

		unsigned int size;
		br.read(&size, 4, 1);
		if(size != br.getSize()) {
			fprintf(stderr, "Bad size\n");
			return;
		}

		br.seek(2, O_CUR); // skip reserved1

		br.read(&keyframeCount, 4, 1);
		if(keyframeCount > TEX_SRT_ANIM_CAPACITY) {
			fprintf(stderr, "Capacity\n");
			return;
		}

		unsigned int metadataOff;
		unsigned int keyframesOff;
		br.read(&metadataOff, 4, 1);
		br.read(&keyframesOff, 4, 1);
		br.seek(0x8, O_CUR); // skip reserved2
		
		unsigned char* metadataFlags = (unsigned char*)zalloc(keyframeCount);
		for(int i = 0; i < keyframeCount; i++) {
			br.read(&metadataFlags[i], 1, 1);
		}

		while((br.tell() % 0x10) > 0) {
			br.seek(1, O_CUR);
		}

		for(int i = 0; i < keyframeCount; i++) {
			std::memset(&keyframes[i], 0, sizeof(TexSrtAnimKeyframe));

			br.read(&keyframes[i].time, 4,1 );
			if(metadataFlags[i] & SRT_HAS_SCALE) {
				br.read(&keyframes[i].scale.x, 4, 1);
				br.read(&keyframes[i].scale.y, 4, 1);
			}
			if(metadataFlags[i] & SRT_HAS_ROTATION) {
				br.read(&keyframes[i].rotationAngle, 4, 1);
			}
			if(metadataFlags[i] & SRT_HAS_TRANSLATION) {
				br.read(&keyframes[i].translation.x, 4, 1);
				br.read(&keyframes[i].translation.y, 4, 1);
			}
			if(metadataFlags[i] & SRT_HAS_SHEAR) {
				br.read(&keyframes[i].shear.x, 4, 1);
				br.read(&keyframes[i].shear.y, 4, 1);
			}
		}

		for(int i = 0; i < keyframeCount-1; i++) {
			keyframes[i].next = &keyframes[i+1];
		}

		free(metadataFlags);

		return;
	}
};
