#pragma once

#include <cstdio>
#include <cstdlib>
#include "BinaryIO.hpp"
#include "Endianness.hpp"
#include "GameSysCore.hpp"
#include "Middle.hpp"
#include "util/Option.hpp"

class BinaryReader {
public:
	BinaryReader() {
		buffer = nullptr;
		size = 0;
		position = 0;
		targetEndianness = E_LITTLE;
		hostEndianness = GameCore.endianness;
	}

	~BinaryReader() {
		if(buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
	}

	void loadFile(const char* filename) {
		std::FILE* fp = std::fopen(filename, "rb");
		if(fp == nullptr) {
			return;
		}

		std::fseek(fp, 0, SEEK_END);
		size = std::ftell(fp);
		std::rewind(fp);

		buffer = new byte[size];
		if(buffer == nullptr) {
			return;
		}

		std::fread(buffer, 1, size, fp);

		std::fclose(fp);
	}

	int read(void* ptr, std::size_t size, std::size_t count) {
		uint readCount = 0;

		for(uint i = 0; i < count; i++) {
			if((this->size - position) < size) {
				break;
			}

			std::memcpy((byte*)ptr + (size*i), buffer + position, size);
			if(targetEndianness != hostEndianness) {
				bswap((byte*)ptr, size);
			}
			position += size;
			readCount += size;
		}

	exit:
		return readCount;
	}

	template<typename T>
	Option<T> readInt(int endianness = -1) {
		int endiannessSave = targetEndianness;
		if(endianness != -1) {
			targetEndianness = endianness;
		}

		Option<T> out = NullOpt;
		T value;
		int result = read(&value, sizeof(T), 1);
		if(result == sizeof(T)) {
			out = value;
		}

		targetEndianness = endiannessSave;
		
		return out;
	}

	uint readLStr(char* str) {
		uint len = readInt<uint>().valueOr();
		read(str, 1, len);

		return len;
	}

	uint readZStr(char* str) {
		uint len = 0;
		char ch = -1;
		while((read(&ch, 1, 1) == 1) && ch != '\0') {
			str[len] = ch;
			len++;	
		}

		return len;
	}

	int readStr(char* str, uint len) {
		std::memset(str, 0, len+1);

		return read(str, len, 1);
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

	int getEndianness() {
		return targetEndianness;
	}

	void setEndianness(int newEndianness) {
		targetEndianness = newEndianness;
	}

private:
	byte* buffer;
	std::size_t size;
	std::size_t position;
	int targetEndianness;
	int hostEndianness;
};
