#pragma once

#include <cstdio>
#include <cstring>

#define TIS_CAPACITY 4096
enum TempIntStorageFlags {
	// slot free flags
	TISF_FREE = 0,
	TISF_LOCKED = 1,
};

struct TempIntStorage {
	int slots[TIS_CAPACITY];
	int capacity;
	int size;

	TempIntStorage() {
		memset(slots, 0, TIS_CAPACITY);
	}

	~TempIntStorage() {

	}

	int getFree(int slotNo) {
		int out = 0;

		printf("slotNo: %d\n", slotNo);

		return out;
	}

	void read(int slotNo) {

	}
};

