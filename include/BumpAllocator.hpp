#pragma once

#include <cstring>
#include "Middle.hpp"

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
		if(store == nullptr) {
			isValid = false;
			isAlloc = false;
			return;
		}
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
		if(head + size > tail) {
			return nullptr;
		}

		head += size;

		return (void*)(head - size);
	}
};
