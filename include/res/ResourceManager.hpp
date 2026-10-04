#pragma once

#include "ResEffect3DManager.hpp"
#include "ResFontManager.hpp"
#include "ResTexManager.hpp"

namespace lvk::res {

class ResourceManager {
public:
	ResFontManager fontMan;
	ResTexManager texMan;
	ResEffect3DManager eff3DMan;

	Font* getRFont(const std::string& key) {
		return &fontMan[key]->font();
	}

	ResFont* getFont(const std::string& key) {
		return fontMan[key];
	}

	Texture* getRTex(const std::string& key) {
		return &texMan[key]->tex();
	}

	ResTex* getTex(const std::string& key) {
		return texMan[key];
	}

	TaskPtc3D* getREff3D(const std::string& key) {
		return &eff3DMan[key]->effect3d();
	}

	ResEffect3D* getEff3D(const std::string& key) {
		return eff3DMan[key];
	}
};

}
