#ifndef MIDDLE_HPP
#define MIDDLE_HPP

#include <cstdint>

typedef uint8_t byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;

#define INIT_SET_STR(str, data, size) \
	memset(str, 0, size); \
	memcpy(str, data, size); \

#endif // MIDDLE_HPP
