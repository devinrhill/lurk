#pragma once

#include <raylib.h>
#include "../../util/Gui.hpp"
#include "../Task.hpp"

class TaskGui: public Task {
public:
    enum State {
        NONE = 0,

        // status 0 - 5
        PRESSED = (1 << 0),
        RELEASED = (1 << 1),
        HOVER = (1 << 2),
        DOWN = (1 << 3),
        UP = (1 << 4),

        // button modifier 6 - 9
        LEFT_BUTTON = (1 << 6),
        RIGHT_BUTTON = (1 << 7),
        
        // device modifier 10 - 11
        MOUSE = (1 << 10),
        KEYBOARD = (1 << 11),

        // special hints 12 - 15
        WANT_ESCAPE = (1<<12),
        WANT_MINIMIZE = (1<<13),
        WANT_MAXIMIZE = (1<<14),
        WANT_RESIZE = (1<<15),

        // persistent status flags
        CAN_MOVE = (1<<0),
        SHOULD_MOVE = (1<<1),
        CAN_RESIZE = (1<<2),
        SHOULD_RESIZE = (1<<3),
        //(1<<4)
        //(1<<5)
        //(1<<6)
        //(1<<7)

        ENTER = (1<<8),
        EXIT = (1<<9),
        REGULAR = (1<<10),
        MINIMIZED = (1<<11),
        MAXIMIZED = (1<<12)
    };

    uint state;
    uint lastState;

    uint persist;
    uint lastPersist;

    bool visible;

    Vector2 trans;
    Vector2 scale;

    Vector2 padding;
    Vector2 margin;

    Color color ;

    bool hasBorder;
    Vector2 border;
    Color borderColor;

    float fontSize;
    Color fontColor;

    TaskGui() {
        flags = PRE_UPDATE | UPDATE | POST_DRAW | DRAW_POST_2D;
        state = 0;
        lastState = 0;
        persist = REGULAR;
        lastPersist = 0;
        visible = true;
        trans = {0, 0};
        scale = {0, 0};
        padding = {10, 10};
        margin = {10, 10};
        color = BLACK;
        hasBorder = false;
        borderColor = WHITE;
        border = {2, 2};
        fontSize = 20;
        fontColor = WHITE;
    }

    bool preUpdate(Task* param) override {
        lastState = state;
        lastPersist = persist;

        state = 0;

        if(visible) {
            if(!(flags & POST_DRAW)) {
                flags |= POST_DRAW;
            }
        } else {
            if(flags & POST_DRAW) {
                flags &= ~POST_DRAW;
            }
        }

        if(isGuiBoxPressedLeft(trans, scale)) {
            state |= PRESSED;
            state |= LEFT_BUTTON;
            state |= MOUSE;
        }
        if(isGuiBoxPressedRight(trans, scale)) {
            state |= PRESSED;
            state |= RIGHT_BUTTON;
            state |= MOUSE;
        }

        if(isGuiBoxReleasedLeft(trans, scale)) {
            state |= RELEASED;
            state |= LEFT_BUTTON;
            state |= MOUSE;
        }
        if(isGuiBoxReleasedRight(trans, scale)) {
            state |= RELEASED;
            state |= RIGHT_BUTTON;
            state |= MOUSE;
        }

        if(isGuiBoxDownLeft(trans, scale)) {
            state |= DOWN;
            state |= LEFT_BUTTON;
            state |= MOUSE;
        }
        if(isGuiBoxDownRight(trans, scale)) {
            state |= DOWN;
            state |= RIGHT_BUTTON;
            state |= MOUSE;
        }

        if(isGuiBoxUpLeft(trans, scale)) {
            state |= UP;
            state |= LEFT_BUTTON;
            state |= MOUSE;
        }
        if(isGuiBoxUpRight(trans, scale)) {
            state |= UP;
            state |= RIGHT_BUTTON;
            state |= MOUSE;
        }

        if(isAABB2D(aabb2D(trans, scale), GetMousePosition())) {
            if(GetKeyPressed() != 0) {
                if(IsKeyPressed(KEY_ESCAPE)) {
                    state |= WANT_ESCAPE;
                }
                if(IsKeyPressed(KEY_MINUS)) {
                    state |= WANT_MINIMIZE;
                }
                state |= KEYBOARD;
                state |= PRESSED;
            }
        }

        // signalling
        if(state & PRESSED) {
            //*onPress()
        }

            // click
        if(state & RELEASED && lastState & PRESSED) {

        }

        return true;
    }

    bool update(Task* param) override {
        if(state & WANT_ESCAPE) {
            if(persist & REGULAR) {
                persist |= EXIT;
            }
        }

        if(state & WANT_MINIMIZE) {
            if(persist & REGULAR) {
                persist |= MINIMIZED;
            } else if(persist & MINIMIZED) {
                persist |= REGULAR;
            }
        }

        if(state & WANT_MAXIMIZE) {
            if(persist & MINIMIZED) {
                persist &= ~MINIMIZED;
                persist |= REGULAR;
            }
        }

        if(persist & EXIT) {
            flags &= 0;
            //visible = false;
        } else if(persist & MINIMIZED || persist & ENTER || persist & REGULAR) {
            visible = true;
        }
        visible = true;

        return true;
    }
};
