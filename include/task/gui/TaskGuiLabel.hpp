// Devin Hill 2026

#pragma once

#include <raylib.h>
#include "TaskGui.hpp"

namespace lvk {

struct TaskGuiLabel: TaskGui {
    const char* text;

    TaskGuiLabel(): TaskGui() {
        setName("TaskGuiLabel");
        flags |= POST_DRAW | DRAW_POST_2D;

        text = "";
    }

    void setText(const char* newText) {
        text = newText;

        scale.x = MeasureText(text, fontSize);
        scale.y = fontSize;
    }

    void postDraw(int status, Task* param) override {
        TaskGui::postDraw(status, param);

        DrawRectangle(trans.x, trans.y, scale.x + padding.x, scale.y + padding.y, color);
        if(hasBorder) {
            DrawRectangleLines(trans.x + border.x/2, trans.y + border.y/2, scale.x - border.x, scale.y - border.y, borderColor);
        }
        DrawText(text, trans.x + padding.x/2, trans.y + padding.y/1.5, fontSize, fontColor);
        
        //DrawRectangleLines(trans.x, trans.y, scale.x, scale.y, WHITE);
    }
};

} // namespace lvk
