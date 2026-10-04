#pragma once

#include <raylib.h>

namespace lvk::res {

class ResTex {
public:
	ResTex() {
		isLoaded = false;
		rTex = {};
	}

	void loadFile(const char* filename) {
		if(!isLoaded) {
			rTex = LoadTexture(filename);
		}
	}

	Texture& tex() {
		return rTex;
	}

	bool loaded() const {
		return isLoaded;
	}

private:
	bool isLoaded;
	Texture rTex;
};

}
