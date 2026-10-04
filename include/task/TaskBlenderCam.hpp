// Devin Hill 2026

#pragma once

#include <cmath>
#include <raylib.h>
#include "../GameSysCore.hpp"
#include "../geo/Vec3.hpp"
#include "../Math.hpp"
#include "../task/Task.hpp"
#include "../util/Raylib.hpp"

using namespace lvk;
using namespace lvk::geo;

namespace lvk {

class TaskBlenderCam : public Task {
public:
	enum CameraFlags {
		UPDATE_POSITION = (1<<0),
		UPDATE_TARGET = (1<<1),
		ORBITAL = (1<<2),
		PAN = (1<<3)
	};

	int camFlags;

	// Persistent camera state
	Vec3 target;
	float orbitRadius;

	float yaw;
	float pitch;

	// Output
	Camera3D* out;

	Vec3 finalPosition;
	Vec3 finalTarget;

	TaskBlenderCam() {
		setName("TaskBlenderCam");
		flags |= PRE_UPDATE | UPDATE | POST_UPDATE;

		camFlags = UPDATE_POSITION | UPDATE_TARGET;

		out = &GameCore.camera3d;

		finalPosition = Vec3(GameCore.camera3d.position);
		finalTarget = Vec3(GameCore.camera3d.target);

		target = finalTarget;

		Vec3 offset = finalPosition - target;

		orbitRadius = offset.length();

		if (orbitRadius < 0.001f)
			orbitRadius = 1.0f;

		Vec3 direction = offset / orbitRadius;

		pitch = std::asinf(
			direction.y = math::clamp<float>(direction.y, -1.0f, 1.0f)
		);

		yaw = std::atan2f(
			direction.x,
			direction.z
		);
	}

	bool preUpdate(Task* param) override {
		float wheel = GetMouseWheelMove();

		if(wheel != 0.0f) {
			if(IsKeyDown(KEY_LEFT_SHIFT)) {
				orbitRadius *= std::pow(0.95f, wheel);
			} else {
				orbitRadius *= std::pow(0.75f, wheel);
			}
		}

		if (
			!IsKeyDown(KEY_LEFT_SHIFT) &&
			IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)
		) {
			camFlags &= ~PAN;
			camFlags |= ORBITAL;

			const float mouseSensitivity = 0.01f;

			yaw -= GameCore.mouseDelta.x * mouseSensitivity;
			pitch += GameCore.mouseDelta.y * mouseSensitivity;

			pitch = math::clamp<float>(
				pitch,
				-PI / 2.0f + mouseSensitivity,
				PI / 2.0f - mouseSensitivity
			);
		}
		else if (
			IsKeyDown(KEY_LEFT_SHIFT) &&
			IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)
		) {
			camFlags &= ~ORBITAL;
			camFlags |= PAN;
		}
		else {
			camFlags &= ~(ORBITAL | PAN);
		}

		return true;
	}

	bool update(Task* param) override {
		Vec3 orbitOffset(
			std::cosf(pitch) * std::sinf(yaw),
			std::sinf(pitch),
			std::cosf(pitch) * std::cosf(yaw)
		);

		Vec3 cameraPosition =
			target + orbitOffset * orbitRadius;

		Vec3 forward = -orbitOffset;

		Vec3 worldUp(0.0f, 1.0f, 0.0f);

		Vec3 right = Vec3(
			forward.z,
			0.0f,
			-forward.x
		);

		if (right.length() > 0.0001f)
			right = right.normalized();

		Vec3 movement(0.0f);

		if (IsKeyDown(KEY_W))
			movement += forward;

		if (IsKeyDown(KEY_S))
			movement -= forward;

		if (IsKeyDown(KEY_D))
			movement -= right;

		if (IsKeyDown(KEY_A))
			movement += right;

		if (movement.length() > 0.0001f)
			movement = movement.normalized();

		const float moveSpeed = 3;

		movement *= moveSpeed * GameCore.dt;

		if(IsKeyDown(KEY_Q)) {
			target.y -= moveSpeed * GameCore.dt;
		}
		if(IsKeyDown(KEY_E)) {
			target.y += moveSpeed * GameCore.dt;
		}
		if(IsKeyPressed(KEY_R)) {
			orbitRadius = 1.0f;
		}

		target += movement;

		if (camFlags & PAN) {
			const float mouseSensitivity = 0.005f;

			Vec3 panRight = Vec3(
				util::getCamera3DRight(GameCore.camera3d)
			);

			Vec3 up = Vec3(
				util::getCamera3DUp(GameCore.camera3d)
			);

			Vec3 delta =
				panRight * (-GameCore.mouseDelta.x * mouseSensitivity) +
				up       * ( GameCore.mouseDelta.y * mouseSensitivity);

			target += delta;
		}

		finalTarget = target;

		finalPosition =
			target + orbitOffset * orbitRadius;

		return true;
	}

	bool postUpdate(Task* param) override {
		if (out == nullptr)
			return true;

		if (camFlags & UPDATE_POSITION)
			out->position = finalPosition.raylib();

		if (camFlags & UPDATE_TARGET)
			out->target = finalTarget.raylib();

		return true;
	}
};

} // namespace lvk
