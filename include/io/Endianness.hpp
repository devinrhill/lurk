// Devin Hill 2026

#pragma once

#include "../core/Types.hpp"

namespace lvk::io {

enum Endianness {
	E_BIG = 0,
	E_LITTLE = 1
};

inline int getEndianness() {
    u32 x = 0x01234567;
    return ((int)(*(byte*)(&x) == 0x67));
}

inline u16 bswap16(u16 value) {
	return ((((value) >> 8) & 0x00FFU) | (((value) << 8) & 0xFF00U));
}

inline u32 bswap32(u32 value) {
	return ((((value) >> 24) & 0x000000FFUL) | (((value) >> 8) & 0x0000FF00UL)
		 | (((value) << 8) & 0x00FF0000UL)   | (((value) << 24) & 0xFF000000UL));
}

inline u64 bswap64(u64 value) {
	return ((((value) >> 56) & 0x00000000000000FFULL) | (((value) >> 40) & 0x000000000000FF00ULL)
		 | (((value) >> 24) & 0x0000000000FF0000ULL)  | (((value) >> 8) & 0x00000000FF000000ULL)
		 | (((value) << 8) & 0x000000FF00000000ULL)   | (((value) << 24) & 0x0000FF0000000000ULL)
		 | (((value) << 40) & 0x00FF000000000000ULL)  | (((value) << 56) & 0xFF00000000000000ULL));
}

inline void bswap(void* value, int size) {
	void* tmp = value;

	switch (size) {
	case 1:
		break;
	case 2:
		*(u16*)value = bswap16(*(u16*)value);
		break;
	case 4:
		*(u32*)value = bswap32(*(u32*)value);

		break;
	case 8:
		*(u64*)value = bswap64(*(u64*)value);

		break;
	default:
		for (int i = 0; i < size; i++) {
			((byte*)tmp)[i] = ((byte*)value)[size - i -1];
		}

		*(byte*)value = *(byte*)tmp;

		break;
	}
}

} // namespace lvk::io
