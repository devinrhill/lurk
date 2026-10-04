// Devin Hill 2026

#pragma once

#include <cstdio>
#include <cstring>
#include "Types.hpp"

namespace lvk {

struct TempIntStorage {
	enum Flags {
		TISF_FREE = 0,
		TISF_LOCKED = 1,
	};

	static constexpr uint CAPACITY = 4096;

	int slots[CAPACITY];
	int capacity;
	int size;

	TempIntStorage() {
		memset(slots, 0, CAPACITY);
	}

	~TempIntStorage() {

	}

	int nextFree(int slotNo) {
		int out = 0;

		for(uint i = 0; i < CAPACITY; i++) {
		}

		printf("slotNo: %d\n", slotNo);

		return out;
	}

	void read(int slotNo) {

	}
};

} // namespace lvk
