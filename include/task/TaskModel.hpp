// Devin Hill 2026

#pragma once

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "core/TaskState.hpp"
#include "../geo/Vec3.hpp"
#include "../anim/TexSrtAnim.hpp"

using namespace lvk::geo;

namespace lvk {

struct TaskModel: TaskState {
    Vec3 position;
    Vec3 scale;
    Quaternion rotation;
    Matrix mtx;
    Color color;
    bool doCalcMtx;
    bool hasSrt;
    TexSrtAnim srt;

    bool visible;
    Model rModel;
    ModelAnimation* rModelCha;
    int chaCount;
    uint chaIdx;
    uint chaFrame;
    bool hasCha;
    bool loadModel;
	bool disableBackface;

    TaskModel() {
        setName("TaskModel");
        flags |= UPDATE | DRAW;
        drawFlags[MAIN] |= DRAW_3D;

		position = Vec3(0);
		scale = Vec3(1);
		color = WHITE;
        visible = true;
        rModel = {0};
        disableBackface = false;
        mtx = MatrixIdentity();
        hasSrt = false;
        hasCha = false;
        doCalcMtx = true;
        chaIdx = 0;
        chaFrame = 0;
        loadModel = false;
        chaCount = 0;
    }

    ~TaskModel() {
    	if(hasCha) {
			UnloadModelAnimations(rModelCha, chaCount);
		}
		if(loadModel) {
			UnloadModel(rModel);
		}
    }

    void loadFile(const char* filename) {
        rModel = LoadModel(filename);
        if(rModel.meshCount > 0) {
			loadModel = true;
        }

        rModelCha = LoadModelAnimations(filename, &chaCount);
        printf("yo\n");
        if(chaCount > 0) {
        	hasCha = true;
        }
    }

    bool update(Task* param) override {
    	if(hasCha) {
    		printf("cha\n");
    		UpdateModelAnimation(rModel, rModelCha[chaIdx], (float)chaFrame);

    		chaFrame++;

    		if(chaFrame >= rModelCha[chaIdx].keyframeCount) {
    			chaFrame = 0;
    		}
    	}
        if(hasSrt) {
			srt.update();
        }
        if(doCalcMtx) {
			calcMtx();
        }

        return true;
    }

    void calcMtx() {
    	/*
		mtx = MatrixIdentity();
		mtx = MatrixMultiply(mtx, MatrixScale(scale.x, scale.y, scale.z));
		mtx = MatrixMultiply(mtx, MatrixRotateXYZ(QuaternionToEuler(rotate)));
		mtx = MatrixMultiply(mtx, MatrixTranslate(trans.x, trans.y, trans.z));
		*/
    }

    void draw(int status, Task* param) override {
        if(visible) {
        	if(hasSrt) {
				srt.push();
        	}

        	if(disableBackface) {
        		rlDisableBackfaceCulling();
        	}
        	/*
        	rlPushMatrix();
        	rlMultMatrixf(MatrixToFloat(mtx));

			rlColor4f(color.r, color.g, color.b, color.a);
			*/
            DrawModelEx(rModel, position.raylib(), Vector3Zero(), 0.0f, Vector3One(), WHITE);

            //rlPopMatrix();

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

} // namespace lvk
