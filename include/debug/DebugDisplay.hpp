// Devin Hill 2026

#pragma once

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <raylib.h>
#include "geo/Vec2.hpp"

namespace lvk {

struct DebugDisplay {
public:
	int stack;
	geo::Vec2 position;
	int fontSize;
	Color color;
	bool active;

	DebugDisplay() {
		stack = 0;
		position = {10, 30};
		fontSize = 20;
		color = WHITE;
		active = true;
	}

	void draw(const char* format, ...) {
		if(active) {
			char buffer[0x100];
			memset(buffer, 0, 0x100);

			va_list ap;
			va_start(ap, format);

			vsnprintf(buffer, 0x100, format, ap);
			DrawText(buffer, position.x, position.y + (stack * fontSize), fontSize, color);
			stack++;

			va_end(ap);
		}
	}

	void reset() {
		if(active) {
			stack = 0;
		}
	}
};

} // namespace lvk
