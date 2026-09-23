#pragma once

#include <raylib.h>
#include <raymath.h>
#include <cstdint>
#include "../util/Util.hpp"

#define CAM_ANIM_CAPACITY 4096

enum CamAnimInterpolationType {
	CAM_INT_LERP = 0,
	CAM_INT_SMOOTH1 = 1,
	CAM_INT_SMOOTH2 = 2,
	CAM_INT_EASEIN = 4,
	CAM_INT_EASEOUT = 5,
	CAM_INT_EXP = 6,
	CAM_INT_SINE = 7,
	CAM_INT_HERMITE_AUTO = 8, // auto tangent
	CAM_INT_HERMITE = 9 // user provided tangents
};

struct CamAnimKeyframe {
	float time;
	Vector3 position;
	float fovy;
	Vector3 rotationEuler;

	int timeInterp;
	int posInterp;
	int fovyInterp;
	int rotInterp;

	uint64_t userData;

	struct CamAnimKeyframe* next;

	CamAnimKeyframe() {
		time = 0.0f;
		position = Vector3Zero();
		fovy = 60.0f;
		rotationEuler = Vector3Zero();
		userData = 0;
		next = NULL;
	}
};

struct CamAnim {
	int keyframeCount;
	struct CamAnimKeyframe* keyframes;

	float elapsed;
	int fps;
	struct CamAnimKeyframe* currFrame;
	int currFrameIdx;

	struct CamAnimKeyframe currState;

	int isPlaying;
	int updateTarget;
	Camera3D* target;

	CamAnim() {
		keyframeCount = 0;
		keyframes = (CamAnimKeyframe*)zalloc(sizeof(struct CamAnimKeyframe) * CAM_ANIM_CAPACITY);
		elapsed = 0.0f;
		fps = 60;
		currFrame = NULL;
		currFrameIdx = 0;
		currState = {};
		isPlaying = 0;
		updateTarget = 0;
		target = NULL;
	}

	~CamAnim() {
		free(keyframes);
	}

	void play() {
		isPlaying = 1;
	}

	void stop() {
		isPlaying = 0;
	}

	void update() {
#if 0
		if(!isPlaying) {
			return;
		}

		elapsed += GetFrameTime() * fps;

		struct CamAnimKeyframe* curr = currFrame;
		struct CamAnimKeyframe* next = curr->next;

		if(next == NULL) {
			currFrame = &keyframes[0];
			currFrameIdx = 0;
			elapsed = 0.0f;
			return;
		}

		if(elapsed >= next->time) {
			currFrame = next;
			currFrameIdx = fwrapp(currFrameIdx + 1, 0, keyframeCount - 1);
			if(elapsed >= keyframes[keyframeCount - 1].time) {
				elapsed = 0.0f;
			}
			curr = currFrame;
			next = curr->next;
			if(next == NULL) {
				return;
			}
		}

		float dur = next->time - curr->time;
		if(dur <= 0.00001f) {
			return;
		}

		float t = fclampp((elapsed - curr->time) / dur, 1.0f);

		int segIdx = (int)(curr - keyframes);

		int it = curr->interpolationType;

		struct CamAnimKeyframe* prev  = (segIdx > 0)
			? &keyframes[segIdx - 1]
			: NULL;
		struct CamAnimKeyframe* next2 = (segIdx + 2 < keyframeCount)
			? &keyframes[segIdx + 2]
			: NULL;

		float segDur = next->time - curr->time;

		Vector3 tangent0;
		float   tangentf0;
		Vector3 tangentRot0;
		if(prev != NULL) {
			float dt0 = next->time - prev->time;
			if(fabsf(dt0) < 1e-6f) dt0 = 1.0f;
			float scale0 = segDur / dt0;
			tangent0    = Vector3Scale(Vector3Subtract(next->position,      prev->position),      scale0);
			tangentf0   = (next->fovy          - prev->fovy)          * scale0;
			tangentRot0 = Vector3Scale(Vector3Subtract(next->rotationEuler, prev->rotationEuler), scale0);
		} else {
			tangent0    = Vector3Subtract(next->position,      curr->position);
			tangentf0   = next->fovy          - curr->fovy;
			tangentRot0 = Vector3Subtract(next->rotationEuler, curr->rotationEuler);
		}

		Vector3 tangent1;
		float   tangentf1;
		Vector3 tangentRot1;
		if(next2 != NULL) {
			float dt1 = next2->time - curr->time;
			if(fabsf(dt1) < 1e-6f) dt1 = 1.0f;
			float scale1 = segDur / dt1;
			tangent1    = Vector3Scale(Vector3Subtract(next2->position,      curr->position),      scale1);
			tangentf1   = (next2->fovy         - curr->fovy)          * scale1;
			tangentRot1 = Vector3Scale(Vector3Subtract(next2->rotationEuler, curr->rotationEuler), scale1);
		} else {
			tangent1    = Vector3Subtract(next->position,      curr->position);
			tangentf1   = next->fovy          - curr->fovy;
			tangentRot1 = Vector3Subtract(next->rotationEuler, curr->rotationEuler);
		}

			currState.position      = v3hermite(curr->position,      next->position,      tangent0,    tangent1,    t);
			currState.fovy          = fhermite( curr->fovy,          next->fovy,          tangentf0,   tangentf1,   t);

			currState.rotationEuler = v3hermite(curr->rotationEuler, next->rotationEuler, tangentRot0, tangentRot1, t);

			switch(it) {
			case CAM_INT_SMOOTH1:  t = stepSmooth1(t);  break;
			case CAM_INT_SMOOTH2:  t = stepSmooth2(t);  break;
			case CAM_INT_EASEIN:   t = stepEaseIn(t);   break;
			case CAM_INT_EASEOUT:  t = stepEaseOut(t);  break;
			case CAM_INT_EXP:      t = stepExp(t);      break;
			case CAM_INT_SINE:    t = stepSine(t);     break;
			case CAM_INT_LERP:
			default:                                     break;
			}

			currState.position      = Vector3Lerp(curr->position,      next->position,      t);
			currState.fovy          = Lerp(       curr->fovy,          next->fovy,          t);
			currState.rotationEuler = Vector3Lerp(curr->rotationEuler, next->rotationEuler, t);
		}

		if(updateTarget) {
			target->position = currState.position;
			target->fovy     = currState.fovy;

			float pitch = currState.rotationEuler.x;
			float yaw   = currState.rotationEuler.y;
			Vector3 forward = {
			 	sinf(yaw) * cosf(pitch),
			 	sinf(pitch),
				-cosf(yaw) * cosf(pitch)
			};
			target->target = Vector3Add(target->position, forward);
			target->up     = getCameraRoll(*target, currState.rotationEuler.z);
		}
#endif
	}

	void load(const char* filename) {

	}
};
