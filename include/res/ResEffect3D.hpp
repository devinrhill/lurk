// Devin Hill 2026

#pragma once

#include "../eff3d/TaskEffect3D.hpp"

namespace lvk::res {

class ResEffect3D {
public:
	ResEffect3D() {
		isLoaded = false;
		rEff3d = {};
	}

	void loadFile(const char* filename) {
		if(!isLoaded) {
			rEff3d.loadFile(filename);
		}
	}

	ef3::TaskEffect3D& effect3d() {
		return rEff3d;;
	}

	bool loaded() const {
		return isLoaded;
	}

private:
	bool isLoaded;
	ef3::TaskEffect3D rEff3d;
};

}
