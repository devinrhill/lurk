#pragma once

#include <cstdint>

enum Endianness {
	E_BIG = 0,
	E_LITTLE = 1
};

static inline int getEndianness() {
    uint32_t x = 0x01234567;
    return ((int)(*(uint8_t*)(&x) == 0x67));
}
static inline uint16_t bswap16(uint16_t value) {
	return ((((value) >> 8) & 0x00FFU) | (((value) << 8) & 0xFF00U));
}

static inline uint32_t bswap32(uint32_t value) {
	return ((((value) >> 24) & 0x000000FFUL) | (((value) >> 8) & 0x0000FF00UL)
		 | (((value) << 8) & 0x00FF0000UL)   | (((value) << 24) & 0xFF000000UL));
}

static inline uint64_t bswap64(uint64_t value) {
	return ((((value) >> 56) & 0x00000000000000FFULL) | (((value) >> 40) & 0x000000000000FF00ULL)
		 | (((value) >> 24) & 0x0000000000FF0000ULL)  | (((value) >> 8) & 0x00000000FF000000ULL)
		 | (((value) << 8) & 0x000000FF00000000ULL)   | (((value) << 24) & 0x0000FF0000000000ULL)
		 | (((value) << 40) & 0x00FF000000000000ULL)  | (((value) << 56) & 0xFF00000000000000ULL));
}

void bswap(void* value, int size) {
	void* tmp = value;

	switch (size) {
	case 1:
		break;
	case 2:
		*(uint16_t*)value = bswap16(*(uint16_t*)value);
		break;
	case 4:
		*(uint32_t*)value = bswap32(*(uint32_t*)value);

		break;
	case 8:
		*(uint64_t*)value = bswap64(*(uint64_t*)value);

		break;
	default:
		for (int i = 0; i < size; i++) {
			((uint8_t*)tmp)[i] = ((uint8_t*)value)[size - i -1];
		}

		*(uint8_t*)value = *(uint8_t*)tmp;

		break;
	}
}
