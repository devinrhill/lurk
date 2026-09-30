#ifndef TASKMODEL_HPP
#define TASKMODEL_HPP

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "TaskStateMachine.hpp"
#include "../geo/Vec3.hpp"
#include "../anim/TexSrtAnim.hpp"

using namespace geo;

struct TaskModel: TaskStateMachine {
    Vec3 trans;
    Vec3 scale;
    Quaternion rotate;
    Matrix mtx;
    Color color;
    bool doCalcMtx;
    bool hasSrt;
    TexSrtAnim srt;

    bool visible;
    Model rModel;
	bool disableBackface;

    TaskModel() {
        setName("TaskModel");
        flags |= UPDATE | DRAW;
        drawFlags[MAIN] |= DRAW_3D;

		trans = Vec3(0);
		scale = Vec3(1);
		color = WHITE;
        visible = true;
        rModel = {0};
        disableBackface = false;
        mtx = MatrixIdentity();
        hasSrt = false;
        doCalcMtx = true;
    }

    void loadFile(const char* filename) {
        rModel = LoadModel(filename);
    }

    bool update(Task* param) override {
        // update anims, shaders
        if(hasSrt) {
			srt.update();
        }
        if(doCalcMtx) {
			calcMtx();
        }
        return true;
    }

    void calcMtx() {
		mtx = MatrixIdentity();
		mtx = MatrixMultiply(mtx, MatrixScale(scale.x, scale.y, scale.z));
		mtx = MatrixMultiply(mtx, MatrixRotateXYZ(QuaternionToEuler(rotate)));
		mtx = MatrixMultiply(mtx, MatrixTranslate(trans.x, trans.y, trans.z));
    }

    void draw(int status, Task* param) override {
        if(visible) {
        	if(hasSrt) {
				srt.push();
        	}

        	if(disableBackface) {
        		rlDisableBackfaceCulling();
        	}
        	rlPushMatrix();
        	rlMultMatrixf(MatrixToFloat(mtx));

			rlColor4f(color.r, color.g, color.b, color.a);
            DrawModelEx(rModel, trans.raylib(), Vector3Zero(), 0.0f, Vector3One(), WHITE);

            rlPopMatrix();

            if(disableBackface) {
            	rlEnableBackfaceCulling();
            }
        }
    }

    void setSrt(TexSrtAnim& anim) {
    	hasSrt = true;
    	srt = anim;
    }
};

#endif // TASKMODEL_HPP
