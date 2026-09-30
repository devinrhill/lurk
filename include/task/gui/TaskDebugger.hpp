#pragma once

#include <raylib.h>
#include <stdio.h>
#include "../../util/Gui.hpp"
#include "../Task.hpp"
#include "../TaskSysCore.hpp"
#include "TaskGuiWindow.hpp"

struct TaskDebugger: public TaskGuiWindow {
    struct TaskInfo {
        Task* task;
        int indent;
    };
    Task* ROOT;
    TaskInfo* tasks;
    uint lastTaskCount;
    uint taskCount;
    static constexpr uint taskMax = 64;
    bool dirty;
    uint map[10] = {PRE_UPDATE, UPDATE, POST_UPDATE, PRE_DRAW, DRAW, POST_DRAW, DRAW_PRE_3D, DRAW_3D, DRAW_2D, DRAW_POST_2D};

    TaskDebugger(): TaskGuiWindow() {
        setName("TaskDebugger");
        flags |= PRE_UPDATE | UPDATE | POST_DRAW;
        drawFlags[POST] = DRAW_POST_2D;

        trans = {50, 50};
        scale = {640, 480};
        color = {0, 0, 0, 150};
        borderColor = WHITE;
        padding = {10, 10};
        fontSize = 20;

        ROOT = &TaskCore.ROOT;
        lastTaskCount = 1;
        taskCount = 0;
        tasks = new TaskInfo[taskMax];
        memset(tasks, 0, sizeof(TaskInfo) * taskMax);
        dirty = true;

        title = "Task Debugger/Manager";
    }

    ~TaskDebugger() {

        delete[] tasks;
    }

    bool preUpdate(Task* param) override {
        TaskGuiWindow::preUpdate(param);

        if(IsKeyPressed(KEY_P)) {
            Task::print(ROOT);
        }

        if(IsKeyPressed(KEY_C)) {
            printf("root tasks: %d\n", Node::getChildCount(ROOT));
        }

        if(IsKeyPressed(KEY_D)) {
            dirty = true;
        }

        if(!(persist & MINIMIZED)) {
            for(int i = 0; i < taskCount; i++) {
                for(int j = 0; j < 10; j++) {
                    if(strcmp(tasks[i].task->name, name)) {
                        if(isGuiBoxPressedLeft({trans.x + padding.x + fontSize * tasks[i].indent + 400 + j * fontSize-4, trans.y + padding.y + fontSize * i + 30}, {fontSize-4, fontSize-4})) {
                            if(tasks[i].task->flags & map[j]) {
                                tasks[i].task->flags &= ~map[j];
                            } else {
                                tasks[i].task->flags |= map[j];
                            }
                        }
                    }
                }
            }
        }

        return true;
    }

    bool update(Task* param) override {
        TaskGuiWindow::update(param);

        lastTaskCount = taskCount;
        taskCount = Node::getChildCount(ROOT);

        if(!dirty) dirty = (lastTaskCount != taskCount);

        if(dirty) {
            memset(tasks, 0, sizeof(TaskInfo) * taskMax);
            taskCount = 0;

            Task* task3;
            Task* task2 = task3 = ROOT;
            while(task3 != nullptr) {
                tasks[taskCount].task = task3;
                tasks[taskCount].indent = 0;
                taskCount++;

                task2 = task3;
                task3 = (Task*)task3->next(ROOT);
            }

            dirty = false;
        }
 
        return true;
    }

    void postDraw(int status, Task* param) override {
        TaskGuiWindow::postDraw(status, param);

        if(!(persist & MINIMIZED)) {
            for(int i = 0; i < taskCount; i++) { 
                char text[0x200];
                strcpy(text, TextFormat("%s %s",((tasks[i].task->flags == 0))?"[.]":"[X]", tasks[i].task->name));
                if(tasks[i].task->flags == 0) {
                    DrawText(text, trans.x + padding.x + fontSize * tasks[i].indent, trans.y + padding.y + fontSize * i + 30, fontSize, MAROON);
                } else {
                    DrawText(text, trans.x + padding.x + fontSize * tasks[i].indent, trans.y + padding.y + fontSize * i + 30, fontSize, LIME);
                }

                for(int j = 0; j < 10; j++) {
                    Color color = BLACK;
                    Color textColor = WHITE;
                    const char* headerText;
                    int displayJ = j;
                    if(j>=0 && j <= 2) {
                        displayJ = j+1;
                        color = BLUE;
                        headerText = "Update";
                    } else if(j>=3 && j<=5) {
                        displayJ = j-2;
                        color = MAROON;
                        headerText = "Draw";
                    } else if(j>=6 && j<=10) {
                        displayJ = j-5;
                        color = GOLD;
                        headerText = "Order";
                    }

                    DrawRectangleLines(trans.x + padding.x + fontSize * tasks[i].indent + 400 + j * fontSize-4 - 1, trans.y + padding.y + fontSize * i + 30 - 1, fontSize-2, fontSize-2, WHITE);
                    bool active = tasks[i].task->flags & map[j];
                    if(active) {
                        DrawRectangle(trans.x + padding.x + fontSize * tasks[i].indent + 400 + j * fontSize-4, trans.y + padding.y + fontSize * i + 30, fontSize-4, fontSize-4, color);
                        textColor = WHITE;
                    } else {
                        //DrawRectangleLines(trans.x + padding.x + fontSize * tasks[i].indent + 400 + j * fontSize-4, trans.y + padding.y + fontSize * i + 30, fontSize-4, fontSize-4, color);
                        DrawRectangle(trans.x + padding.x + fontSize * tasks[i].indent + 400 + j * fontSize-4, trans.y + padding.y + fontSize * i + 30, fontSize-4, fontSize-4, {color.r, color.g, color.b, (byte)(color.a/3)});
                        textColor = GRAY;
                    }

                    if(j == 0 || j == 3 || j == 6) {
                        DrawRectangle(trans.x + padding.x + fontSize * tasks[i].indent + 400 + 1 + j * fontSize-4 - 1 - 5, trans.y + padding.y + 30 - 1 - 10 - 1, MeasureText(headerText, 12) + 10, 10, WHITE);
                        DrawText(headerText, trans.x + padding.x + fontSize * tasks[i].indent + 400 + 1 + j * fontSize-4, trans.y + padding.y + 30 - 1 - 10, 12, color);
                    }

                    DrawText(TextFormat("%d", displayJ), trans.x + padding.x + fontSize * tasks[i].indent + 400 + 1 + j * fontSize-4, trans.y + padding.y + fontSize * i + 30 - 1, fontSize, textColor);
                }

                float width = MeasureText(text, fontSize);
                //DrawRectangleLines(trans.x + padding.x + fontSize * tasks[i].indent, trans.y + padding.y + fontSize * i + 30, width, fontSize, WHITE);
                if(isGuiBoxPressedLeft({trans.x + padding.x + fontSize * tasks[i].indent, trans.y + padding.y + fontSize * i + 30}, {width, fontSize})) {
                    if(tasks[i].task->flags == 0) {
                        tasks[i].task->flags = UPDATE_ALL | DRAW_ALL | DRAW_POST_2D;
                    } else {
                        if(strcmp(tasks[i].task->name, name)) { // dont let user remove task debugger
                            tasks[i].task->flags = 0;
                        }
                    }
                }

                //DrawRectangleLines(trans.x + padding.x + fontSize * tasks[i].indent, trans.y + padding.y + fontSize * i + 30, width, fontSize, WHITE);
            }
        }
    }
};
