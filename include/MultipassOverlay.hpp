#pragma once

#define MULTIPASS_OVERLAY_CAPACITY 128

#include <raylib.h>
#include <cstdlib>
#include "util/Util.hpp"
#include "GameSysCore.hpp"

struct MultipassOverlay {
	int shaderCount;
	Shader** shaders;

	RenderTexture2D rt[2];
	int rtUsing;

	MultipassOverlay() {
		shaderCount = 0;
		shaders = nullptr;
		rtUsing = 0;
	}

	void init() {
		shaders = (Shader**)zalloc(sizeof(Shader*) * MULTIPASS_OVERLAY_CAPACITY);
		rt[0] = LoadRenderTexture(GameCore.wctx.width, GameCore.wctx.height);
		rt[1] = LoadRenderTexture(GameCore.wctx.width, GameCore.wctx.height);
	}

	void close() {
		UnloadRenderTexture(rt[0]);
		UnloadRenderTexture(rt[1]);
		free(shaders);
	}

	void push(Shader* shader) {
		if(shaderCount+1 > MULTIPASS_OVERLAY_CAPACITY) {
			return;
		}
		shaders[shaderCount] = shader;
		shaderCount++;
	}

	void begin() {
		if(shaderCount == 0) {
			BeginDrawing();
			return;
		}
		rtUsing = 0;

		BeginTextureMode(rt[rtUsing]);
	}

	void end() {
		if(shaderCount == 0) {
			EndDrawing();
			return;
		}

		EndTextureMode();

		rtUsing = !rtUsing;
		for(int i = 0; i < shaderCount; i++) {
			BeginTextureMode(rt[rtUsing]);
				ClearBackground(BLACK);

				rtUsing = !rtUsing;
				BeginShaderMode(*shaders[i]);
					DrawTextureRec(rt[rtUsing].texture, (Rectangle){0.0f, 0.0f, (float)rt[rtUsing].texture.width, (float)-rt[rtUsing].texture.height}, (Vector2){0.0f, 0.0f}, WHITE);
				EndShaderMode();
			EndTextureMode();
		}

		rtUsing = !rtUsing;

		BeginDrawing();
			ClearBackground(BLACK);

			DrawTextureRec(rt[rtUsing].texture, (Rectangle){0.0f, 0.0f, (float)rt[rtUsing].texture.width, (float)-rt[rtUsing].texture.height}, (Vector2){0.0f, 0.0f}, WHITE);
		EndDrawing();
	}
};
