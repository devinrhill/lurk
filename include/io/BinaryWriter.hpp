// Devin Hill 2026

#pragma once

#include <cstddef>
#include <cstdio>
#include "BinaryIO.hpp"
#include "Endianness.hpp"
#include "../core/GameSysCore.hpp"
#include "../core/Types.hpp"
#include "../util/Util.hpp"

namespace lvk::io {

class BinaryWriter {
public:
	BinaryWriter() {
		buffer = nullptr;
		size = 0;
		capacity = 1;
		position = 0;
		targetEndianness = E_LITTLE;
		hostEndianness = GameCore.endianness;
	}

	~BinaryWriter() {
		if(buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
	}

	void dumpFile(const char* filename) {
		std::FILE* fp = std::fopen(filename, "wb");
		if(fp == nullptr) {
			return;
		}

		if(std::fwrite(buffer, 1, size, fp) != size) {
			return;
		}

		std::fclose(fp);
	}

	uint write(void* ptr, std::size_t size, std::size_t count) {
		uint wrote = 0;

		for(unsigned int i = 0; i < count; i++) {
			_grow(size);

			std::memcpy(buffer + position, ptr, size);

			if(targetEndianness != hostEndianness) {
				bswap(buffer + position, size);
			}

			position += size;
			this->size += size;
			wrote += size;
		}

		return wrote;
	}

	template<typename T>
	uint writeInt(T x, int endianness = -1) {
		int endiannessSave = targetEndianness;
		if(endianness != -1) {
			targetEndianness = endianness;
		}

		int result = write(&x, sizeof(T), 1);

		targetEndianness = endiannessSave;

		return result;
	}



	uint writeLStr(char* str, int len) {
		int useLen = len;
		if(len == -1) {
			useLen = std::strlen(str);
		}

		uint wrote = 0;
		wrote += writeInt<uint>(useLen);
		wrote += write(str, 1, useLen);

		return wrote;
	}

	uint writeZStr(char* str, int len) {
		int useLen = len;
		if(len == -1) {
			useLen = std::strlen(str);
		}

		uint wrote = 0;
		wrote += write(str, 1, useLen);

		char tmp = '\0';
		wrote += write(&tmp, 1, 1);

		return wrote;
	}

	uint pad(byte ch, uint align) {
		uint count = 0;

		while(((position + count) % align) > 0) {
			count++;
		}

		return write(&ch, 1, count);
	}

	bool seek(std::size_t pos, int origin) {
		std::size_t npos = size;

		switch(origin) {
		case O_SET:
			npos = pos;
			break;
		case O_CUR:
			npos = position + pos;
			break;
		case O_END:
			npos = size - pos;
			break;
		}

		if(npos < size) {
			position = npos;
			return true;
		}

		return false;
	}

	std::size_t tell() const {
		return position;
	}

	const byte* getBuffer() const {
		return buffer;
	}

	std::size_t getSize() const {
		return size;
	}

	void setEndianness(int newEndianness) {
		targetEndianness = newEndianness;
	}

private:
	byte* buffer;
	std::size_t size;
	std::size_t capacity;
	std::size_t position;
	int targetEndianness;
	int hostEndianness;

	void _grow(std::size_t size) {
		if(capacity - position < size) {
			while(capacity - position < size) {
				capacity *= 2;
			}
			buffer = (byte*)zrealloc(buffer, capacity);
		}
	}
};

} // namespace lvk::io
