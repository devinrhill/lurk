// Devin Hill 2026

#pragma once

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "Core.hpp"
#include "../Geometry.hpp"
#include "../util/Raylib.hpp"

using namespace lvk::geo;

namespace lvk {

struct TaskModelPrim: TaskState {
	enum PrimType: int {
		CUBE = 1, // mtx
		PLANE = 2, // mtx
		SPHERE_HI = 3,
		SPHERE_LO = 4,
		BILLBOARD_CYLINDER = 5,
		BILLBOARD_SPHERE = 6,
		CAPSULE = 8, // mtx
		PYRAMID = 9 // mtx
	};

	enum PrimFlags: int {
		SOLID = (1<<0),
		WIRES = (1<<1)
	};

    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    float radius;
    Color color;
    bool useTexture;
    Texture* rTexture;
    Mesh rMesh;
    Model rModel;

    bool visible;
    int primType;
    int lastPrimType;
    int primFlags;
    int lastPrimFlags;

    TaskModelPrim() {
        setName("TaskModelPrim");
        flags |= UPDATE | POST_UPDATE | DRAW;
        drawFlags[MAIN] |= DRAW_3D;

        position = Vec3(0.0f);
        rotation = Vec3(0.0f);
        scale = Vec3(1.0f);
        radius = 1.0f;
		color = WHITE;
		useTexture = false;
		rTexture = nullptr;
		rMesh = {};
		rModel = {};

        visible = true;
        primType = CUBE;
        primFlags = SOLID;
    }

    void initMesh() {
    	if(rMesh.vertexCount > 0) {
    		UnloadMesh(rMesh);
    	}
    	if(rModel.meshCount > 0) {
    		UnloadModel(rModel);
    	}

        switch(primType) {
        case CUBE:
			rMesh = GenMeshCube(scale.x, scale.y, scale.z);
        	break;
        case PLANE:
        	rMesh = GenMeshPlane(scale.x, scale.z, 1.0f, 1.0f);
			break;
		case PYRAMID:
			rMesh = GenMeshCylinder(radius, scale.y, 16);
			break;
		case SPHERE_HI:
			rMesh = GenMeshSphere(radius, 32, 32);
			break;
		case SPHERE_LO:
			rMesh = GenMeshSphere(radius, 16, 12);
			break;
		}

		rModel = LoadModelFromMesh(rMesh);

		if(useTexture && rTexture != nullptr) {
			if(rModel.materialCount > 0) {
				rModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *rTexture;
			}
		}
	}

	bool update(Task* param) override {
		if((lastPrimType != primType) || (lastPrimFlags != primFlags)) {
			initMesh();
		}

		return true;
	}

	bool postUpdate(Task* param) override {
		lastPrimType = primType;
		lastPrimFlags = primFlags;

		return true;
	}

    void draw(int status, Task* param) override {
        if(visible) {
        	bool useMtx = (primType == CUBE || primType == PLANE || primType == PYRAMID);

			if(useMtx) {
        		Matrix mtx = MatrixIdentity();
				mtx = MatrixMultiply(mtx, MatrixScale(scale.x, scale.y, scale.z));
        		mtx = MatrixMultiply(mtx, MatrixRotateXYZ(rotation.raylib()));
        		mtx = MatrixMultiply(mtx, MatrixTranslate(position.x, position.y, position.z));

				rlPushMatrix();
				rlMultMatrixf(MatrixToFloat(mtx));

        		if(primType == CAPSULE) {
					if(primFlags & SOLID) {
						DrawCapsule(Vec3(0.0f, 1.0f, 0.0f).raylib(), Vec3(0.0f).raylib(), 1.0f, 16, 12, color);
					} else if(primFlags & WIRES) {
						DrawCapsuleWires(Vec3(0.0f, 1.0f, 0.0f).raylib(), Vec3(0.0f).raylib(), 1.0f, 16, 12, color);
					}
				} else {
					if(primFlags & SOLID) {
						DrawModelEx(rModel, Vec3(0.0f).raylib(), Vec3(0.0f).raylib(), 0.0f, Vec3(1.0f).raylib(), color);
					} else if(primFlags & WIRES) {
						DrawModelWiresEx(rModel, Vec3(0.0f).raylib(), Vec3(0.0f).raylib(), 0.0f, Vec3(1.0f).raylib(), color);
					}
				}

        		rlPopMatrix();
        	} else {
				if(primType == SPHERE_HI || primType == SPHERE_LO) {
					if(primFlags & SOLID) {
						DrawModelEx(rModel, Vec3(0.0f).raylib(), Vec3(0.0f).raylib(), 0.0f, Vec3(1.0f).raylib(), color);
					} else if(primFlags & WIRES) {
						DrawModelWiresEx(rModel, Vec3(0.0f).raylib(), Vec3(0.0f).raylib(), 0.0f, Vec3(1.0f).raylib(), color);
					}
				} else {
					Vec3 up = {0, 1, 0};
					if(primType == BILLBOARD_SPHERE) {
						Vec3 fwd = lvk::util::getCamera3DForward(GameCore.camera3d);
						Vec3 rgt = lvk::util::getCamera3DRight(GameCore.camera3d);
						up = rgt.cross(fwd); // fix cross
					}

					if(primType == BILLBOARD_CYLINDER || primType == BILLBOARD_SPHERE) {
						if(useTexture && rTexture != nullptr) {
							DrawBillboardPro(GameCore.camera3d, *rTexture, (Rectangle){0, 0, (float)rTexture->width, (float)rTexture->height}, position.raylib(), up.raylib(), (Vector2){scale.x, scale.z}, Vec2(.5, .5).raylib(), rotation.x, color);
						}
					}
				}
        	}
        }
    }

    void bindTexture(Texture* tex) {
    	useTexture = true;
    	rTexture = tex;
    }
};

} // namespace lvk
