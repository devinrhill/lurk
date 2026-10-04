// Devin Hill 2026

#pragma once

#include <map>
#include <string>
#include "ResFont.hpp"

namespace lvk::res {

class ResFontManager {
public:
	struct _ResFontData {
		ResFont* res = nullptr;
		bool ownsRes = false;
	};

	std::map<std::string, _ResFontData> storage;

	~ResFontManager() {
		for(auto& it: storage) {
			if(it.second.ownsRes) {
				delete it.second.res;
			}
		}
	}

	void bindCreate(std::string key, const char* filename) {
		ResFont* font = new ResFont;
		font->loadFile(filename);

		bind(key, font, true);
	}

	ResFont* bind(std::string key, ResFont* res, bool isOwning = false) {
		storage[key].res = res;
		storage[key].ownsRes = isOwning;

		return operator[](key);
	}

	ResFont* operator[](const std::string& key) {
		return storage[key].res;
	}
};

}
