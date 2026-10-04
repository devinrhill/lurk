#pragma once

#include <map>
#include <string>
#include "ResTex.hpp"

namespace lvk::res {

class ResTexManager {
public:
	struct _ResTexData {
		ResTex* res = nullptr;
		bool ownsRes = false;
	};

	std::map<std::string, _ResTexData> storage;

	~ResTexManager() {
		for(auto& it: storage) {
			if(it.second.ownsRes) {
				delete it.second.res;
			}
		}
	}

	void bindCreate(const std::string& key, const char* filename) {
		ResTex* tex = new ResTex;
		tex->loadFile(filename);

		bind(key, tex, true);
	}

	ResTex* bind(const std::string& key, ResTex* res, bool isOwning = false) {
		storage[key].res = res;
		storage[key].ownsRes = isOwning;

		return operator[](key);
	}

	ResTex* operator[](const std::string& key) {
		return storage[key].res;
	}
};

}
