#pragma once

#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include "Task.hpp"
#include "../util/Math.hpp"
#include "../util/Raylib.hpp"
#include "../GameSysCore.hpp"

enum CamCtrlFlags {
	CCF_UPDATE_POS = 1 << 0,
	CCF_UPDATE_LOOKAT = 1 << 1,
	CCF_UPDATE_EULER_ROT = 1 << 2,
	CCF_UPDATE_ORBIT_ANGLE = 1 << 3,
	CCF_UPDATE_ORBIT_POS = 1 << 4,
	CCF_LOOK_PITCH = 1 << 5,
	CCF_LOOK_YAW = 1 << 6,
	CCF_ROLL = 1 << 7,
	CCF_FORWARD = 1 << 8,
	CCF_RIGHT = 1 << 9,
	CCF_UP = 1 << 10,
	CCF_WRAP_ANGLES = 1 << 11
};

enum CamCtrlMode {
	CCM_STATIC = (CCF_UPDATE_POS | CCF_UPDATE_LOOKAT | CCF_WRAP_ANGLES),
	CCM_ORBIT = (CCF_UPDATE_ORBIT_ANGLE | CCF_UPDATE_ORBIT_POS | CCF_WRAP_ANGLES),
	CCM_FPS = (CCF_UPDATE_EULER_ROT | CCF_LOOK_PITCH | CCF_LOOK_YAW | CCF_ROLL | CCF_WRAP_ANGLES),
	CCM_FPS_MOVE = (CCF_UPDATE_EULER_ROT | CCF_LOOK_PITCH | CCF_LOOK_YAW | CCF_ROLL | CCF_FORWARD | CCF_RIGHT | CCF_UP | CCF_WRAP_ANGLES)
};

class TaskCamCtrl: public Task {
public:
	int camFlags = 0;
	Camera3D* cam = nullptr;

	// orbital
	Vector3 origin = {0, 0, 0};
	Vector3 lookat = {0, 0, 0};
	float angle = 0.0f;
	float distance = 0.0f;
	float y = 0.0f;
	float speed = 1.0f;

	Vector3 rotation = {0, 0, 0};

	Vector2 sensitivity = {1, 1};
	Vector3 lastRotation = {0, 0, 0};

	TaskCamCtrl() {
		setName("TaskCamCtrl");
		flags = UPDATE;
	}

	bool update(Task* param) {
		Vector2 mouseDelta = GetMouseDelta();
		const float pitchLimit = 88.0f * DEG2RAD;
		Vector3 dir = Vector3Zero();

		if(camFlags & CCF_UPDATE_POS) {
			cam->position = origin;
		}
		if(camFlags & CCF_UPDATE_LOOKAT) {
			cam->target = lookat;
		}
		if(camFlags & CCF_UPDATE_ORBIT_ANGLE) {
			angle += GameCore.dt * speed;
		}
		if(camFlags & CCF_UPDATE_ORBIT_POS) {
			cam->position.x = distance * cosf(angle) + origin.x;
			cam->position.y = y + origin.y;
			cam->position.z = distance * sin(angle) + origin.z;
		}
		if(camFlags & CCF_LOOK_YAW) {
			mouseDelta.x *= sensitivity.x;

			rotation.x += mouseDelta.y * GameCore.dt;
		}
		if(camFlags & CCF_LOOK_PITCH) {
			mouseDelta.y *= sensitivity.y;

			rotation.y -= mouseDelta.x * GameCore.dt;
		}
		if(camFlags & CCF_ROLL) {
			if(IsKeyDown(KEY_C)) {
				rotation.z += speed * GameCore.dt;
			}
			if(IsKeyDown(KEY_Z)) {
				rotation.z -= speed * GameCore.dt;
			}

			cam->up = getCameraRoll(*cam, rotation.z);
		}
		if(camFlags & CCF_FORWARD) {
			if(IsKeyDown(KEY_W)) {
				dir = Vector3Add(
					dir,
					getRotationForward(rotation.x, rotation.y)
				);
			}
			if(IsKeyDown(KEY_S)) {
				dir = Vector3Subtract(
					dir,
					getRotationForward(rotation.x, rotation.y)
				);
			}
		}
		if(camFlags & CCF_RIGHT) {
			if(IsKeyDown(KEY_D)) {
				dir = Vector3Add(
					dir,
					Vector3CrossProduct(
						getRotationForward(rotation.x, rotation.y),
						cam->up
					)
				);
			}
			if(IsKeyDown(KEY_A)) {
				dir = Vector3Subtract(
					dir,
					Vector3CrossProduct(
						getRotationForward(rotation.x, rotation.y),
						cam->up
					)
				);
			}
		}
		if(camFlags & CCF_UP) {
			if(IsKeyDown(KEY_Q)) {
				dir = Vector3Add(
					dir,
					Vector3Negate(
						cam->up
					)
				);
			}
			if(IsKeyDown(KEY_E)) {
				dir = Vector3Add(
					dir,
					cam->up
				);
			}
		}

		if(Vector3Length(dir) > 0.0f) {
			dir = Vector3Normalize(dir);

			cam->position = Vector3Add(
				cam->position,
				Vector3Scale(
					Vector3Scale(
						dir,
						GameCore.dt
					),
					speed
				)
			);
		}
		if(camFlags & CCF_UPDATE_EULER_ROT) {
			camera3DRotateEuler(cam, rotation);
		}
		if(camFlags & CCF_WRAP_ANGLES) {
			pfwrap(&angle, 0.0f, 2.0f * M_PI);
			pfclamp(&rotation.x, -pitchLimit, pitchLimit);
			pfwrap(&rotation.y, 0.0f, 2.0f * M_PI);
			pfwrap(&rotation.z, 0.0f, 2.0f * M_PI);
		}

		return true;
	}
};
