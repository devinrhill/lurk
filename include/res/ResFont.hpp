#pragma once

#include <raylib.h>

namespace lvk::res {

class ResFont {
public:
	ResFont() {
		isLoaded = false;
		rFont = {};
	}

	void loadFile(const char* filename) {
		if(!isLoaded) {
			rFont = LoadFont(filename);
		}
	}

	Font& font() {
		return rFont;
	}

	bool loaded() const {
		return isLoaded;
	}
	
private:
	bool isLoaded;
	Font rFont;
};

}
