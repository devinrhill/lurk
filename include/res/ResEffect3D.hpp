#pragma once

#include "../task/TaskPtc3D.hpp"

namespace lvk::res {

class ResEffect3D {
public:
	ResEffect3D() {

	}

	void loadFile(const char* filename) {
		if(!isLoaded) {
			rPtc3d.loadFile(filename);
		}
	}

	TaskPtc3D& effect3d() {
		return rPtc3d;
	}

	bool loaded() const {
		return isLoaded;
	}

private:
	bool isLoaded;
	TaskPtc3D rPtc3d;
};

}
