#ifndef GUITEXTENTRY_HPP
#define GUITEXTENTRY_HPP

#include <raylib.h>
#include "../../Middle.hpp"
#include "TaskGui.hpp"

struct GuiTextEntry: TaskGui {
	char buffer[0x100];
	uint length;
	bool complete;

	GuiTextEntry() {
		setName("GuiTextEntry");
		flags |= Task::Flags::PRE_UPDATE | Task::Flags::DRAW | Task::Flags::DRAW_2D;
		memset(buffer, 0, 0x100);
		length = 0;
		complete = false;
	}

	bool preUpdate(Task* param) {
		int c = GetCharPressed();
		while (c > 0) {
			if (length < 0x100 - 1 &&
				c >= 32 && c <= 126) {
				buffer[length++] = (char)c;
				buffer[length] = '\0';
			}

			c = GetCharPressed();
		}

		if(IsKeyPressed(KEY_ENTER) && length > 0) {
			complete = 1;
		}

		if (IsKeyPressed(KEY_BACKSPACE) && length > 0) {
			buffer[--length] = '\0';
		}

		return true;
	}

	void draw(int status, Task* param) {
		DrawText(buffer, 10, 10, 20, MAROON);
	}
};

#endif // GUITEXTENTRY_HPP
