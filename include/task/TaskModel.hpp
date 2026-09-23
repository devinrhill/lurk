#ifndef TASKMODEL_HPP
#define TASKMODEL_HPP

#include <raylib.h>
#include <raymath.h>
#include "TaskStateMachine.hpp"

struct TaskModel: TaskStateMachine {
    Vector3 trans;
    Vector3 scale;
    Color color;

    bool visible;
    Model rModel;

    TaskModel() {
        setName("TaskModel");
        flags = UPDATE | DRAW | DRAW_3D;

        visible = true;
        rModel = {0};
    }

    void loadFile(const char* filename) {
        rModel = LoadModel(filename);
    }

    bool update(Task* param) override {
        // update anims, shaders
        return true;
    }

    void draw(int status, Task* param) override {
    	printf("TaskModel::draw\n");
        if(visible) {
            DrawModelEx(rModel, trans, Vector3Zero(), 0.0f, scale, color);
        }
    }
};

#endif // TASKMODEL_HPP
