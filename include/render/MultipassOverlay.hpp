// Devin Hill 2026

#pragma once

#define MULTIPASS_OVERLAY_CAPACITY 128

#include <raylib.h>
#include "WindowCtx.hpp"
#include "geo/Vec2.hpp"

namespace lvk {

struct MultipassOverlay {
	int shaderCount;
	Shader** shaders;
	bool isOwning;
	geo::Vec2 screenSize;

	RenderTexture2D rt[2];
	int rtUsing;

	MultipassOverlay() {
		shaderCount = 0;
		shaders = nullptr;
		rtUsing = 0;
		isOwning = false;
		screenSize = geo::Vec2(0.0f);
	}

	void init(WindowCtx wctx) {
		shaders = new Shader*[MULTIPASS_OVERLAY_CAPACITY];
		for(uint i = 0; i < shaderCount; i++) {
			shaders[i] = nullptr;
		}
		this->screenSize = geo::Vec2(wctx.width, wctx.height);
		rt[0] = LoadRenderTexture(screenSize.x, screenSize.y);
		rt[1] = LoadRenderTexture(screenSize.x, screenSize.y);
	}

	void close() {
		UnloadRenderTexture(rt[0]);
		UnloadRenderTexture(rt[1]);
		if(isOwning) {
			for(uint i = 0; i < shaderCount; i++) {
				delete shaders[i];
			}
		}
		delete[] shaders;
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

} // namespace lvk
