// Devin Hill 2026

#pragma once

#include <raylib.h>

namespace lvk {

struct ShaderCtx {
	Shader texSrtAnim;
	Shader texSprAnim;
	Shader alphaDiscard;
	Shader alphaDiscardBlack;
	Shader blackAlpha;
	Shader light;
	Shader blinnPhong;

	void init() {
		texSrtAnim = LoadShader(nullptr, "res/shader/tex_srt_anim.fs");
		texSprAnim = LoadShader(nullptr, "res/shader/tex_spr_anim.fs");
		alphaDiscard = LoadShader(nullptr, "res/shader/alpha_discard.fs");
		alphaDiscardBlack = LoadShader(nullptr, "res/shader/alpha_discard_black.fs");
		blackAlpha = LoadShader(nullptr, "res/shader/black_alpha.fs");
		light = LoadShader("res/shader/lighting.vs", "res/shader/lighting.fs");
		blinnPhong = LoadShader("res/shader/blinn_phong.vs", "res/shader/blinn_phong.fs");
	}

	void close() {
		UnloadShader(texSrtAnim);
		UnloadShader(texSprAnim);
		UnloadShader(alphaDiscard);
		UnloadShader(alphaDiscardBlack);
		UnloadShader(blackAlpha);
		UnloadShader(light);
	}
};

} // namespace lvk
