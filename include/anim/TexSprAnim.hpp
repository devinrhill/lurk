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
#include <vector>

#define TEX_SPR_ANIM_CAPACITY 16

static bool checkVersion(int version) {
	switch(version) {
	case 2:
	case 3:
	case 4:
		return true;
	}

	return false;
}

static constexpr int versionWork[] = {2, 3};

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

	void init() {
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
		outputTexture = {};
		isPlaying = false;

		translationLoc = -1;
		scaleLoc = -1;
	}

	void close() {
		if(ownsName) {
			delete[] name;
		}
		if(ownsTexturePath) {
			delete[] texturePath;
		}
		if(ownsTexture) {
			UnloadTexture(outputTexture);
		}
		if(frameInfo != NULL) {
			for(int i = 0; i < frameCount; i++) {
				if(frameInfo[i].name != NULL) {
					if(frameInfo[i].ownsName) {
						delete[] frameInfo[i].name;
					}
				}
			}
			delete[] frameInfo;
		}
	}

	int load(const char* filename) {
		if(filename == NULL) {
			return 1;
		}

		getUniforms();

		struct BinaryReader br;
		br.loadFile(filename);

		char magic[4];
		br.read(magic, 4, 1);
		if(memcmp(magic, "\x89SPR", 4)) {
			fprintf(stderr, "TexSprAnim load: Invalid magic: '%08x'\n", magic);
			return 1;
		}

		char endianness;
		br.read(&endianness, 1, 1);
		if(endianness == 0) {
			br.setEndianness(E_BIG);
		} else if(endianness == 1) {
			br.setEndianness(E_LITTLE);
		} else {
			fprintf(stderr, "TexSprAnim load: Invalid endianness: '%d'\n", endianness);
			return 1;
		}

		char version;
		br.read(&version, 1, 1);
		if(!checkVersion(version)) {
			fprintf(stderr, "TexSprAnim load: Invalid version: '%d', expected: '%d'\n", version, 1);
			return 1;
		}

		unsigned int size = br.readInt<uint>().valueOr();
		if(size != br.getSize()) {
			fprintf(stderr, "TexSprAnim load: Invalid file size, doesn't match real file size (%d != %d)\n", size, br.getSize());
			return 1;
		}

		frameWidth = br.readInt<ushort>().valueOr();
		frameHeight = br.readInt<ushort>().valueOr();
		orientation = br.readInt<ushort>().valueOr();
		frameCount = br.readInt<uint>().valueOr();

		int frameInfoOff = br.readInt<uint>().valueOr();
		int masterNameOff = br.readInt<uint>().valueOr();
		int texturePathOff = br.readInt<uint>().valueOr();

		br.seek(frameInfoOff, O_SET);

		frameInfo = (TexSprAnimFrameInfo*)zalloc(sizeof(struct TexSprAnimFrameInfo) * frameCount);
		for(int i = 0; i < frameCount; i++) {
			int nameOff = br.readInt<uint>().valueOr();
			frameInfo[i].length = br.readInt<float>().valueOr();

			int seekback = br.tell();
			br.seek(nameOff, O_SET);
			int nameLen = br.readInt<uint>().valueOr();
			frameInfo[i].name = (char*)zalloc(nameLen+1);
			br.read(frameInfo[i].name, nameLen, 1);
			br.seek(seekback, O_SET);
		}

		br.seek(masterNameOff, O_SET);
		int masterNameLen = br.readInt<uint>().valueOr();
		name = (char*)zalloc(masterNameLen+1);
		br.read(name, masterNameLen, 1);
		ownsName = 1;

		br.seek(texturePathOff, O_SET);
		int texturePathLen = br.readInt<uint>().valueOr();
		texturePath = (char*)zalloc(texturePathLen+1);
		br.read(texturePath, texturePathLen, 1);
		ownsTexturePath = 1;

		outputTexture = LoadTexture(texturePath);
		ownsTexture = 1;

		return 1;
	}

	void update() {
		lastFrame = currentFrame;

		float duration = (1.0f / fps) *
			frameInfo[currentFrame].length;

		if(isPlaying) {
			elapsedTime += GameCore.dt;

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

	static TexSprAnim create(const char* filename) {
		TexSprAnim anim;

		anim.init();
		anim.load(filename);

		return anim;
	}
};

struct TexSprAnimManager {
	int animCount;
	TexSprAnim* anims;

	void init() {
		animCount = 0;
		anims = new TexSprAnim[TEX_SPR_ANIM_CAPACITY];
	}

	void close() {
		for(int i = 0; i < animCount; i++) {
			anims[i].close();
		}

		if(anims != nullptr) {
			delete[] anims;
		}
	}

	void update() {
		for(int i = 0; i < animCount; i++) {
			anims[i].update();
		}
	}

	TexSprAnim* get(const char* name) {
		for(int i = 0; i < animCount; i++) {
			if(!strcmp(anims[i].name, name)) {
				return &anims[i];
			}
		}

		return nullptr;
	}

	struct TexSprAnim* pushBack(struct TexSprAnim anim) {
		if(animCount+1 > TEX_SPR_ANIM_CAPACITY) {
			return nullptr;
		}

		anims[animCount] = anim;
		animCount++;

		return &anims[animCount-1];
	}
};
