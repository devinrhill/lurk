// Devin Hill 2026

#pragma once

#include <cstdio>
#include <cstring>
#include "Types.hpp"
#include "util/Util.hpp"

namespace lvk {

class BumpAllocator {
public:
	bool isValid;
	bool isAlloc;
	std::size_t size;
	byte* store;
	byte* head;
	byte* tail;

	BumpAllocator() {
		isValid = false;
		isAlloc = false;
		size = 0;
		store = nullptr;
		head = nullptr;
		tail = nullptr;
	}

	~BumpAllocator() {
		close();
	}

	void init(std::size_t size) {
		this->size = size;
		store = new byte[size];
		isAlloc = true;

		head = store;
		tail = store + size;

		isValid = true;
	}

	void close() {
		if(isAlloc) {
			std::memset(store, 0, size);
			delete[] store;
			isAlloc = false;
		}

		size = 0;
		store = nullptr;
		head = nullptr;
		tail = nullptr;

		isValid = false;
	}

	void* alloc(std::size_t size) {
		std::size_t travel = util::align(size, sizeof(void*));
		std::size_t waste = travel - size;
#if 0
		printf("bump alloc, req size: %lu, given size: %lu, wasted bytes: %lu\n", size, travel, waste);
#endif

		if(head + travel > tail) {
			return nullptr;
		}

		head += travel;

		return (void*)(head - travel);
	}
};

} // namespace lvk
