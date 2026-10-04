// Devin Hill 2026

#pragma once

#include "../../util/Raylib.hpp"
#include "TaskGui.hpp"

namespace lvk {

struct TaskGuiError: public TaskGui {
    float duration;
    Color color;
    float elapsed;
    bool born;
    bool alive;
    const char* text;

    TaskGuiError(): TaskGui() {
        setName("TaskGuiError");
        flags |= UPDATE | POST_DRAW | DRAW_POST_2D;

        trans = {0, 0};
        duration = 2.0f;
        color = {255, 0, 0, 255};
        elapsed = 0.0f;
        born = true;
        alive = false;
        text = "Error";
        scale = {(float)MeasureText(text, fontSize), fontSize};
    }

    bool update(Task* param) override {
        if(born) {
            alive = true;
            born = false;
        }

        if(alive) {
            elapsed += GameCore.dt;
            alive = (elapsed < duration);
        }

        return true;
    }

    void postDraw(int status, Task* param) override {
        if(alive) {
            int f = 255 - (elapsed / duration) * 255;
            DrawRectangle(trans.x, trans.y, scale.x, scale.y, modAlpha(color, f));
            DrawRectangleLines(trans.x + 1, trans.y + 1, scale.x - 2, scale.y - 2, modAlpha(WHITE, f));
            DrawText(text, trans.x + 1, trans.y + 1, fontSize, modAlpha(WHITE, f));
        }
    }
};

} // namespace lvk
