#ifndef TASKGUIHBOX_HPP
#define TASKGUIHBOX_HPP

#if 0
#include <math.h>
#include "TaskGui.hpp"

struct TaskGuiHBox: public TaskGui {
    TaskGui childRoot;
    bool dirty;

    TaskGuiHBox(): TaskGui() {
        setName("TaskGuiHBox");
        flags |= UPDATE | POST_DRAW | DRAW_POST_2D;

        dirty = true;
    };

    static void _update(Task* param) {
        // int mut x
        // float mut task3->trans.x
        // float mut task3->trans.y
        // Task* const task3
        // float const margin.x
        // float const task3->margin.x
        // float const trans.x
        // float const trans.y
        // float const padding.y
        // float const task3->scale.x

        int* x; // 0
        float marginx; // 1
        Task* task3; // 2
        float* task3marginx; // 3
        float* task3transx; // 4
        float* task3transy; // 5
        float* transx; // 6
        float* transy; // 7
        float* paddingy; // 8
        float* task3scalex; // 9

        x += margin.x + task3->margin.x;

        task3->trans.x = x;
        task3->trans.y = trans.y + padding.y;

        x += task3->scale.x;
    }

    bool update(Task* param) override {
        if(dirty) {
            Task::iter((Task*)Node::next(&childRoot), param, &_update);
            dirty = false;
        }

        return true;
    }

    void postDraw(int status, Task* param) override {
        TaskGui::postDraw(status, param);

        Vector2 scale = calcSize();
        DrawRectangle(trans.x, trans.y, scale.x, scale.y, {0, 128, 255, 128});

        Task::postDrawAll(&childRoot, status, param, DRAW_POST_2D);
    }

    Vector2 calcSize() {
        Vector2 out = {0, 0};

        TaskGui* task3;
        TaskGui* task2 = task3 = &childRoot;
        while(task3 != nullptr) {
            out.x += task3->scale.x + task3->margin.x*3;
            out.y = fmax(out.y, 
                task3->scale.y + task3->margin.y*2);

            task2 = task3;
            task3 = (TaskGui*)task3->next(&childRoot);
        }

        out.y += padding.y * 2;

        return out;
    }

    void add(Task* task) {
        childRoot.connect(task);

        child = childRoot.child;
        task->parent = this;
    }
};
#endif

#endif // TASKGUIHBOX_HPP
