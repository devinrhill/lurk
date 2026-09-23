#pragma once

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "../io/BinaryIO.hpp"
#include "../io/BinaryReader.hpp"
#include "../io/Endianness.hpp"
#include "../util/Util.hpp"
#include "../GameSysCore.hpp"
#include <raylib.h>
#include <raymath.h>

#define TEX_SPR_ANIM_CAPACITY 16
#define TEX_SPR_ANIM_VERSION 3

enum TexSprAnimOrientation {
	SPR_HORIZONTAL = 0,
	SPR_VERTICAL = 1,
	SPR_ATLAS = 2
};

struct TexSprAnimFrameInfo {
	char* name;
	float length;

	int ownsName;
};

struct TexSprAnim {
	// identifiers
	char* name;
	int ownsName;
	char* texturePath;
	int ownsTexturePath;
	int ownsTexture;

	// tex spr info
	int frameWidth;
	int frameHeight;
	int orientation;
	int frameCount;
	struct TexSprAnimFrameInfo* frameInfo;

	// runtime info
	Vector2 uvOffset;
	Vector2 uvScale;
	float fps;
	float elapsedTime;
	int lastFrame;
	int currentFrame;
	Texture2D outputTexture;
	int isPlaying;

	// shader uniforms
	int translationLoc;
	int scaleLoc;

	TexSprAnim() {
		name = nullptr;
		ownsName = 0;
		texturePath = nullptr;
		ownsTexturePath = 0;
		ownsTexture = 0;

		frameWidth = 0;
		frameHeight = 0;
		orientation = SPR_HORIZONTAL;
		frameCount = 0;
		frameInfo = NULL;

		uvOffset = Vector2Zero();
		uvScale = Vector2Zero();
		fps = 1.0f;
		elapsedTime = 0.0f;
		lastFrame = 0;
		currentFrame = 0;
		outputTexture = (Texture2D){0};
		isPlaying = 0;

		translationLoc = -1;
		scaleLoc = -1;
	}

	~TexSprAnim() {
		if(ownsName) {
			free(name);
		}
		if(ownsTexturePath) {
			free(texturePath);
		}
		if(ownsTexture) {
			UnloadTexture(outputTexture);
		}
		if(frameInfo != NULL) {
			for(int i = 0; i < frameCount; i++) {
				if(frameInfo[i].name != NULL) {
					if(frameInfo[i].ownsName) {
						free(frameInfo[i].name);
					}
				}
			}
			free(frameInfo);
		}
	}

	void load(const char* filename) {
		if(filename == NULL) {
			return;
		}

		getUniforms();

		struct BinaryReader br;
		br.loadFile(filename);

		char magic[4];
		br.read(magic, 4, 1);
		if(memcmp(magic, "\x89SPR", 4)) {
			fprintf(stderr, "TexSprAnim load: Invalid magic: '%08x'\n", magic);
			return;
		}

		char endianness;
		br.read(&endianness, 1, 1);
		if(endianness == 0) {
			br.setEndianness(E_BIG);
		} else if(endianness == 1) {
			br.setEndianness(E_LITTLE);
		} else {
			fprintf(stderr, "TexSprAnim load: Invalid endianness: '%d'\n", endianness);
			return;
		}

		char version;
		br.read(&version, 1, 1);
		if(version != TEX_SPR_ANIM_VERSION) {
			fprintf(stderr, "TexSprAnim load: Invalid version: '%d', expected: '%d'\n", version, TEX_SPR_ANIM_VERSION);
			return;
		}

		unsigned int size;
		br.read(&size, 4, 1);
		if(size != br.getSize()) {
			fprintf(stderr, "TexSprAnim load: Invalid file size, doesn't match real file size (%d != %d)\n", size, br.getSize());
			return;
		}

		br.read(&frameWidth, 2, 1);
		br.read(&frameHeight, 2, 1);
		br.read(&orientation, 2, 1);
		br.read(&frameCount, 4, 1);
		int frameInfoOff;
		br.read(&frameInfoOff, 4, 1);
		int masterNameOff;
		br.read(&masterNameOff, 4, 1);
		int texturePathOff;
		br.read(&texturePathOff, 4, 1);

		br.seek(frameInfoOff, O_SET);

		frameInfo = (TexSprAnimFrameInfo*)zalloc(sizeof(struct TexSprAnimFrameInfo) * frameCount);
		for(int i = 0; i < frameCount; i++) {
			int nameOff;
			br.read(&nameOff, 4, 1);
			br.read(&frameInfo[i].length, 4, 1);

			int seekback = br.tell();
			br.seek(nameOff, O_SET);
			int nameLen;
			br.read(&nameLen, 4, 1);
			frameInfo[i].name = (char*)zalloc(nameLen+1);
			br.read(frameInfo[i].name, nameLen, 1);
			br.seek(seekback, O_SET);
		}

		br.seek(masterNameOff, O_SET);
		int masterNameLen;
		br.read(&masterNameLen, 4, 1);
		name = (char*)zalloc(masterNameLen+1);
		br.read(name, masterNameLen, 1);
		ownsName = 1;

		br.seek(texturePathOff, O_SET);
		int texturePathLen;
		br.read(&texturePathLen, 4, 1);
		texturePath = (char*)zalloc(texturePathLen+1);
		br.read(texturePath, texturePathLen, 1);
		ownsTexturePath = 1;

		outputTexture = LoadTexture(texturePath);
		ownsTexture = 1;

		return;
	}

	void update() {
		lastFrame = currentFrame;

		float duration = (1.0f / fps) *
			frameInfo[currentFrame].length;

		if(isPlaying) {
			elapsedTime += GetFrameTime();

			while(elapsedTime >= duration) {
				elapsedTime -= duration;
				currentFrame++;

				if(currentFrame >= frameCount) {
					currentFrame = 0;
				}

				duration = (1.0f / fps) *
					frameInfo[currentFrame].length;
			}
		}

		updateUV();
	}

	void updateUV() {
		int currFrame = currentFrame;
		int frameCount = frameCount;

		float frameRes = 1.0f / frameCount;

		switch(orientation) {
		case SPR_HORIZONTAL:
			uvOffset = (Vector2){currFrame * frameRes, 0.0f};
			uvScale = (Vector2){1.0f / frameCount, 1.0f};
			break;
		case SPR_VERTICAL:
			uvOffset = (Vector2){0.0f, currFrame * frameRes};
			uvScale = (Vector2){1.0f, 1.0f / frameCount};
			break;
		}

	}

	void getUniforms() {
		translationLoc = GetShaderLocation(GameCore.shctx.texSprAnim, "translation");
		scaleLoc = GetShaderLocation(GameCore.shctx.texSprAnim, "scale");
	}

	struct TexSprAnimFrameInfo* getFrame(const char* frameName) {
		for(int i = 0; i < frameCount; i++) {
			if(!strcmp(frameInfo[i].name, frameName)) {
				return &frameInfo[i];
			}
		}

		return NULL;

	}

	void pushUniforms() {
		SetShaderValue(GameCore.shctx.texSprAnim, translationLoc, &uvOffset, SHADER_UNIFORM_VEC2);
		SetShaderValue(GameCore.shctx.texSprAnim, scaleLoc, &uvScale, SHADER_UNIFORM_VEC2);
	}

	void play() {
		isPlaying = 1;
	}

	void stop() {
		isPlaying = 0;
	}
};

struct TexSprAnimManager {
	int animCount;
	struct TexSprAnim* anims;

	TexSprAnimManager() {
		animCount = 0;
		anims = (TexSprAnim*)zalloc(sizeof(struct TexSprAnim) * TEX_SPR_ANIM_CAPACITY);
	}

	~TexSprAnimManager() {
		if(anims != NULL) {
			free(anims);
		}
	}

	void update() {
		for(int i = 0; i < animCount; i++) {
			anims[i].update();
		}
	}

	struct TexSprAnim* get(const char* name) {
		for(int i = 0; i < animCount; i++) {
			if(!strcmp(anims[i].name, name)) {
				return &anims[i];
			}
		}

		return NULL;
	}

	struct TexSprAnim* pushBack(struct TexSprAnim anim) {
		if(animCount+1 > TEX_SPR_ANIM_CAPACITY) {
			return NULL;
		}

		anims[animCount] = anim;
		animCount++;

		return &anims[animCount-1];
	}
};
