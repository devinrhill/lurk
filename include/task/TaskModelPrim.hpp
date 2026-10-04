// Devin Hill 2026

#pragma once

#include <raylib.h>
#include <raymath.h>
#include "core/TaskState.hpp"

namespace lvk {

struct TaskModelPrim: TaskState {
	enum PrimType: int {
		P_CUBE = 1
	};

    Vector3 trans;
    Vector3 scale;
    Color color;

    bool visible;
    int primType;

    TaskModelPrim() {
        setName("TaskModelPrim");
        flags |= DRAW | DRAW_3D;

        visible = true;
    }

    void draw(int status, Task* param) override {
        if(visible) {
        	switch(primType) {
        	case P_CUBE:
        		DrawCubeV(trans, scale, color);
        		break;
        	}
        }
    }
};

} // namespace lvk
