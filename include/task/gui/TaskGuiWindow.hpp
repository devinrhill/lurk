#ifndef TASKGUIWINDOW_HPP
#define TASKGUIWINDOW_HPP

#include <raylib.h>
#include <raymath.h>
#include "../../Debug.hpp"
#include "../Task.hpp"
#include "TaskGui.hpp"

// BUG: Window cannot be closed when minimized

struct TaskGuiWindow: public TaskGui {
    const char* title;
    Vector2 ogScale;
    bool drawWindowCtrl;
    bool drawWindowTitle;
    Vector2 initDragTrans; // me every tuesday
    Vector2 dragTrans;

    TaskGuiWindow(): TaskGui() {
        setName("TaskGuiWindow");

        persist |= CAN_MOVE | CAN_RESIZE;
        title = "";
        ogScale = {0, 0};
        drawWindowCtrl = true;
        drawWindowTitle = true;
        initDragTrans = {-1, -1};
        dragTrans = {-1, -1};
    }

    bool preUpdate(Task* param) override {
        TaskGui::preUpdate(param);

        int st = scale.x-80;
        if(isGuiBoxPressedLeft({trans.x - padding.x + scale.x - st, trans.y - padding.y + scale.y - fontSize}, {fontSize, fontSize})) {
            (persist&CAN_MOVE)?persist&=~CAN_MOVE:persist|=CAN_MOVE;
        }
        if(isGuiBoxPressedLeft({trans.x - padding.x + scale.x - st+fontSize, trans.y - padding.y + scale.y - fontSize}, {fontSize, fontSize})) {
            (persist&SHOULD_MOVE)?persist&=~SHOULD_MOVE:persist|=SHOULD_MOVE;
        }
        if(isGuiBoxPressedLeft({trans.x - padding.x + scale.x - st+fontSize*2, trans.y - padding.y + scale.y - fontSize}, {fontSize, fontSize})) {
            (persist&CAN_RESIZE)?persist&=~CAN_RESIZE:persist|=CAN_RESIZE;
        }
        if(isGuiBoxPressedLeft({trans.x - padding.x + scale.x - st+fontSize*3, trans.y - padding.y + scale.y - fontSize}, {fontSize, fontSize})) {
            (persist&SHOULD_RESIZE)?persist&=~SHOULD_RESIZE:persist|=SHOULD_RESIZE;
        }

        if(isGuiBoxPressedLeft({trans.x - padding.x + scale.x - st+fontSize*4, trans.y - padding.y + scale.y - fontSize}, {fontSize, fontSize})) {
            (persist&ENTER)?persist&=~ENTER:persist|=ENTER;
        }
        if(isGuiBoxPressedLeft({trans.x - padding.x + scale.x - st+fontSize*5, trans.y - padding.y + scale.y - fontSize}, {fontSize, fontSize})) {
            (persist&EXIT)?persist&=~EXIT:persist|=EXIT;
        }
        if(isGuiBoxPressedLeft({trans.x - padding.x + scale.x - st+fontSize*6, trans.y - padding.y + scale.y - fontSize}, {fontSize, fontSize})) {
            (persist&REGULAR)?persist&=~REGULAR:persist|=REGULAR;
        }
        if(isGuiBoxPressedLeft({trans.x - padding.x + scale.x - st+fontSize*7, trans.y - padding.y + scale.y - fontSize}, {fontSize, fontSize})) {
            (persist&MINIMIZED)?persist&=~MINIMIZED:persist|=MINIMIZED;
        }
        if(isGuiBoxPressedLeft({trans.x - padding.x + scale.x - st+fontSize*8, trans.y - padding.y + scale.y - fontSize}, {fontSize, fontSize})) {
            (persist&MAXIMIZED)?persist&=~MAXIMIZED:persist|=MAXIMIZED;
        }

        // init original scale for returning after window minimization
        if(ogScale.x == 0 && ogScale.y == 0) {
            if(!(persist & MINIMIZED)) {
                ogScale = scale;
            }
        }

        // escape button
        if(isGuiBoxPressedLeft({trans.x + padding.x, trans.y + padding.y}, {14, 14})) {
            state |= WANT_ESCAPE;
        }

        // minimize button
        if(isGuiBoxPressedLeft({trans.x + padding.x + 18, trans.y + padding.y}, {14, 14})) {
            state |= WANT_MINIMIZE;
        }

        if(isGuiBoxPressedLeft({trans.x + padding.x + 36, trans.y + padding.y}, {14, 14})) {
            state |= WANT_MAXIMIZE;
        }

        if(!(state & WANT_ESCAPE) && !(state & WANT_MINIMIZE) && !(state & WANT_MAXIMIZE)) {
            // titlebar dragging
            if(isGuiBoxPressedLeft(trans, {scale.x, fontSize + padding.y})) {
                if(visible) {
                    if(!(persist & SHOULD_MOVE)) {
                        initDragTrans = GetMousePosition();
                        dragTrans = Vector2Subtract(initDragTrans, trans);
                        //printf("%.1f %.1f %.1f %.1f\n", initDragTrans.x, initDragTrans.y, dragTrans.x, dragTrans.y);
                        persist |= SHOULD_MOVE;
                    }
                }
            }

            if(persist & SHOULD_MOVE) {
                if(isGuiBoxReleasedLeft(trans, {scale.x, fontSize + padding.y})) {
                    persist &= ~SHOULD_MOVE;
                }
            }

            DrawRectangle(trans.x + scale.x - 12, trans.y + scale.y - 12, 10, 10, WHITE);

            if(isGuiBoxDownLeft({trans.x + scale.x - 12, trans.y + scale.y - 12}, {10, 10})) {
                if(!(persist & SHOULD_RESIZE) && !(persist & MINIMIZED)) {
                    if(lastPersist == persist) {
                        initDragTrans = GetMousePosition();
                        dragTrans = Vector2Subtract(initDragTrans, trans);
                        persist |= SHOULD_RESIZE;
                    }
                }
            }

            if(!(persist & MINIMIZED)) {
                persist |= CAN_RESIZE;
            } else {
                persist &= ~CAN_RESIZE;
            }

            if(persist & SHOULD_RESIZE && !(persist & MINIMIZED)) {
                if(isGuiBoxReleasedLeft({trans.x + scale.x - 12, trans.y + scale.y - 12}, {10, 10})) {
                    persist &= ~SHOULD_RESIZE;
                    ogScale = scale;
                }
            }
        }

        return true;
    }

    bool update(Task* param) override {
        TaskGui::update(param);

        if(state & WANT_ESCAPE) {
        }

        if(persist & MINIMIZED) {
            scale.x = ogScale.x;
            scale.y = fontSize + padding.y * 1.75;
        } else {
            scale = ogScale;
        }

        if(persist & CAN_MOVE && persist & SHOULD_MOVE) {
            trans = GetMousePosition();
            trans.x -= dragTrans.x;
            trans.y -= dragTrans.y;
        }

        if(persist & CAN_RESIZE && persist & SHOULD_RESIZE) {
            scale.x += GetMousePosition().x - initDragTrans.x;
            scale.y += GetMousePosition().y - initDragTrans.y;
        }

        return true;
    }

    void postDraw(int status, Task* param) override {
        // window background
        DrawRectangle(trans.x, trans.y, scale.x, scale.y, color);
        DrawRectangleLines(trans.x+2, trans.y+2, scale.x-4, scale.y-4, borderColor);

        if(drawWindowCtrl) {
            // escape button
            DrawRectangle(trans.x + padding.x, trans.y + padding.y, 14, 14, MAROON);
            DrawRectangleLines(trans.x + padding.x - 1, trans.y + padding.y - 1, 16, 16, WHITE);
            DrawLine(trans.x + padding.x - 1 + 4, trans.y + padding.y - 1 + 4, trans.x + padding.x + 16 - 1 - 4, trans.y + padding.y + 16 - 1 - 4, WHITE);
            DrawLine(trans.x + padding.x + 16 - 1 - 4, trans.y + padding.y - 1 + 4, trans.x + padding.x - 1 + 4, trans.y + padding.y + 16 - 1 - 4, WHITE);

            // minimize button
            DrawRectangle(trans.x + padding.x + 18, trans.y + padding.y, 14, 14, GOLD);
            DrawRectangleLines(trans.x + padding.x + 18 - 1, trans.y + padding.y - 1, 16, 16, WHITE);
            DrawLine(trans.x + padding.x + 3 + 18 - 1, trans.y + padding.y + 8 - 1, trans.x + padding.x + 16 - 3 + 18 - 1, trans.y + padding.y + 8 - 1, WHITE);

            // maximize button
            DrawRectangle(trans.x + padding.x + 36, trans.y + padding.y, 14, 14, LIME);
            DrawRectangleLines(trans.x + padding.x + 36 - 1, trans.y + padding.y - 1, 16, 16, WHITE);
            DrawRectangleLines(trans.x + padding.x + 36 - 1 + 4 - 1, trans.y + padding.y - 1 + 4, 10, 8, WHITE);
            DrawRectangle(trans.x + padding.x + 36 - 1 + 4 - 1, trans.y +padding.y - 1 + 4, 10, 3, WHITE);
        }

        // resize button
        if(persist & CAN_RESIZE) {
            DrawRectangle(trans.x + scale.x - 12, trans.y + scale.y - 12, 10, 10, WHITE);
        }

        int st = scale.x-80;
        if(persist & CAN_MOVE) {
            DrawRectangle(trans.x - padding.x + scale.x - st, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, BLUE);
            DrawText("1", trans.x - padding.x + scale.x - st, trans.y - padding.y + scale.y - fontSize, fontSize, WHITE);
        } else {
            DrawRectangleLines(trans.x - padding.x + scale.x - st, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, BLUE);
        }

        if(persist & SHOULD_MOVE) {
            DrawRectangle(trans.x - padding.x + scale.x - st+fontSize, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, BLUE);
            DrawText("2", trans.x - padding.x + scale.x - st+fontSize, trans.y - padding.y + scale.y - fontSize, fontSize, WHITE);
        } else {
            DrawRectangleLines(trans.x - padding.x + scale.x - st+fontSize, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, BLUE);
        }

        if(persist & CAN_RESIZE) {
            DrawRectangle(trans.x - padding.x + scale.x - st+fontSize*2, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, BLUE);
            DrawText("3", trans.x - padding.x + scale.x - st+fontSize*2, trans.y - padding.y + scale.y - fontSize, fontSize, WHITE);
        } else {
            DrawRectangleLines(trans.x - padding.x + scale.x - st+fontSize*2, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, BLUE);
        }

        if(persist & SHOULD_RESIZE) {
            DrawRectangle(trans.x - padding.x + scale.x - st+fontSize*3, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, BLUE);
            DrawText("4", trans.x - padding.x + scale.x - st+fontSize*3, trans.y - padding.y + scale.y - fontSize, fontSize, WHITE);
        } else {
            DrawRectangleLines(trans.x - padding.x + scale.x - st+fontSize*3, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, BLUE);
        }

        if(persist & ENTER) {
            DrawRectangle(trans.x - padding.x + scale.x - st+fontSize*4, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, GOLD);
            DrawText("5", trans.x - padding.x + scale.x - st+fontSize*4, trans.y - padding.y + scale.y - fontSize, fontSize, WHITE);
        } else {
            DrawRectangleLines(trans.x - padding.x + scale.x - st+fontSize*4, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, GOLD);
        }

        if(persist & EXIT) {
            DrawRectangle(trans.x - padding.x + scale.x - st+fontSize*5, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, RED);
            DrawText("6", trans.x - padding.x + scale.x - st+fontSize*5, trans.y - padding.y + scale.y - fontSize, fontSize, WHITE);
        } else {
            DrawRectangleLines(trans.x - padding.x + scale.x - st+fontSize*5, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, RED);
        }

        if(persist & REGULAR) {
            DrawRectangle(trans.x - padding.x + scale.x - st+fontSize*6, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, GREEN);
            DrawText("7", trans.x - padding.x + scale.x - st+fontSize*6, trans.y - padding.y + scale.y - fontSize, fontSize, WHITE);
        } else {
            DrawRectangleLines(trans.x - padding.x + scale.x - st+fontSize*6, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, GREEN);
        }

        if(persist & MINIMIZED) {
            DrawRectangle(trans.x - padding.x + scale.x - st+fontSize*7, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, GOLD);
            DrawText("8", trans.x - padding.x + scale.x - st+fontSize*7, trans.y - padding.y + scale.y - fontSize, fontSize, WHITE);
        } else {
            DrawRectangleLines(trans.x - padding.x + scale.x - st+fontSize*7, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, GOLD);
        }

        if(persist & MAXIMIZED) {
            DrawRectangle(trans.x - padding.x + scale.x - st+fontSize*8, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, LIME);
            DrawText("9", trans.x - padding.x + scale.x - st+fontSize*8, trans.y - padding.y + scale.y - fontSize, fontSize, WHITE);
        } else {
            DrawRectangleLines(trans.x - padding.x + scale.x - st+fontSize*8, trans.y - padding.y + scale.y - fontSize, fontSize, fontSize, LIME);
        }

        if(drawWindowTitle) {
            // window title
            DrawText(title, trans.x + scale.x - padding.x - MeasureText(title, fontSize), trans.y + padding.y, fontSize, WHITE);
#if !NDEBUG
            DrawRectangleLines(trans.x, trans.y, scale.x, fontSize + padding.y, WHITE);
#endif
        }
    }
};

#endif // TASKGUIWINDOW_HPP
