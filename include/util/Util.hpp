#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

inline int g__zero;
inline int g__one;
#define TMPS_CAPACITY 0x20
inline uint64_t g__tmps[TMPS_CAPACITY];

#define TMPS_PGET(i) (&g__tmps[i])
#define TMPS_GET(i) (g__tmps[i])

#define TMPS_PSET(v, i) (g__tmps[(i)] = (v), &g__tmps[(i)])
#define TMPS_SET(v, i) \
	do { \
		g__tmps[i] = v; \
	} while(0)

#define ALIGN(x, a) (x + a - 1) / a * a;
#define ALIGN_PERFECT(x, a) (x + a - 1) & ~(a - 1);
static inline uint32_t alignNext(uint32_t x) {
    if (x == 0) {
		return 1;
	} else {
		x--; x |= x >> 1; x |= x >> 2; x |= x >> 4; x |= x >> 8; x |= x >> 16;
		return x + 1;
	}
}

#define FLAG_ON(flags, flag) (flags | flag)
#define FLAG_OFF(flags, flag) (flags & ~flag)

void* zalloc(size_t size) {
	void* ptr = malloc(size);
	if(ptr == NULL) {
		return NULL;
	}

	memset(ptr, 0, size);

	return ptr;
}

// why would this be needed
void* zrealloc(void* ptr, size_t size) {
	ptr = realloc(ptr, size);
	if(ptr == NULL) {
		return NULL;
	}

	memset(ptr, 0, size);

	return ptr;
}
