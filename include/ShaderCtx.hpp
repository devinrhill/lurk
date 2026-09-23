#pragma once

#include <raylib.h>

struct ShaderCtx {
	Shader texSrtAnim;
	Shader texSprAnim;
	Shader alphaDiscard;
	Shader alphaDiscardBlack;
	Shader blackAlpha;

	void init() {
		texSrtAnim = LoadShader(nullptr, "res/shader/tex_srt_anim.fs");
		texSprAnim = LoadShader(nullptr, "res/shader/tex_srt_anim.fs");
		alphaDiscard = LoadShader(nullptr, "res/shader/alpha_discard.fs");
		alphaDiscardBlack = LoadShader(nullptr, "res/shader/alpha_discard_black.fs");
		blackAlpha = LoadShader(nullptr, "res/shader/black_alpha.fs");
	}

	void close() {
		UnloadShader(texSrtAnim);
		UnloadShader(texSprAnim);
		UnloadShader(alphaDiscard);
		UnloadShader(alphaDiscardBlack);
		UnloadShader(blackAlpha);
	}
};
