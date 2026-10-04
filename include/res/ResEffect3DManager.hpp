#pragma once

#include <map>
#include <string>
#include "ResEffect3D.hpp"

namespace lvk::res {

class ResEffect3DManager {
public:
	struct _ResEffect3DData {
		ResEffect3D* res = nullptr;
		bool ownsRes = false;
	};

	std::map<std::string, _ResEffect3DData> storage;

	~ResEffect3DManager() {
		for(auto& it: storage) {
			if(it.second.ownsRes) {
				delete it.second.res;
			}
		}
	}

	void bindCreate(const std::string& key, const char* filename) {
		ResEffect3D* eff = new ResEffect3D;
		eff->loadFile(filename);

		bind(key, eff, true);
	}

	ResEffect3D* bind(const std::string& key, ResEffect3D* res, bool isOwning = false) {
		storage[key].res = res;
		storage[key].ownsRes = isOwning;
		
		return operator[](key);
	}

	ResEffect3D* operator[](const std::string& key) {
		return storage[key].res;
	}
};

}
