#pragma once

// Devin Hill 2026

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace util {

// temporary uint64_t's
constexpr int TMPS_CAPACITY = 0x20;
inline std::uint64_t g__tmps[TMPS_CAPACITY];

inline std::uint64_t* tmpsPGet(int i) {
	return &g__tmps[i];
}

inline std::uint64_t tmpsGet(int i) {
	return g__tmps[i];
}

inline std::uint64_t* tmpsPSet(std::uint64_t v, int i) {
	g__tmps[i] = v;
	return &g__tmps[i];
}

inline std::uint64_t tmpsSet(std::uint64_t v, int i) {
	g__tmps[i] = v;
	return g__tmps[i];
}

// constants

inline int g__zero = 0;
inline int g__one = 1;
inline float g__fOne = 1.0f;
inline double g__dOne = 1.0;

// bit stuff

inline int align(int x, int a) {
	return (x + a - 1) / a * a;
}
inline int alignPerfect(int x, int a) {
	return (x + a - 1) & ~(a - 1);
}

inline std::uint32_t alignNext(std::uint32_t x) {
    if (x == 0) {
		return 1;
	} else {
		x--; x |= x >> 1; x |= x >> 2; x |= x >> 4; x |= x >> 8; x |= x >> 16;
		return x + 1;
	}
}

inline void flagOn(int& flags, int flag) {
	flags |= flag;
}

inline void flagOff(int& flags, int flag) {
	flags &= ~flag;
}

// memory alloc

inline void* zalloc(std::size_t size) {
	void* ptr = std::malloc(size);
	if(ptr == nullptr) {
		return nullptr;
	}

	std::memset(ptr, 0, size);

	return ptr;
}

} // namespace util
