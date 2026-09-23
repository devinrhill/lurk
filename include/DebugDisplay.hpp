#pragma once

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <raylib.h>

struct DebugDisplay {
public:
	int stack;
	Vector2 position;
	int fontSize;
	Color color;
	bool active;

	DebugDisplay() {
		stack = 0;
		position.x = 10;
		position.y = 30;
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
