#pragma once

#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "Geometry.hpp"
#include "Math.hpp"
#include "Raylib.hpp"

struct IVector2 {
	int x;
	int y;
};

#define GET_VECTOR3_AXIS_MACRO(vec, axis) ((float*)&vec+axis)

bool isAABB2D(struct AABB2D aabb, Vector2 pos) {
	return ((pos.x > aabb.center.x - aabb.halfSize.x && pos.x < aabb.center.x + aabb.halfSize.x && pos.y > aabb.center.y - aabb.halfSize.y && pos.y < aabb.center.y + aabb.halfSize.y));
}

bool isAABB3D(struct AABB3D aabb, Vector3 pos) {
	return ((pos.x > aabb.center.x - aabb.halfSize.x && pos.x < aabb.center.x + aabb.halfSize.x && pos.y > aabb.center.y - aabb.halfSize.y && pos.y < aabb.center.y + aabb.halfSize.y && pos.z > aabb.center.z - aabb.halfSize.z && pos.z < aabb.center.z + aabb.halfSize.z));
}

Color modAlpha(Color c, int alpha) {
    Color n = c;
    n.a = alpha;
    return n;
}

struct AABB2D aabb2D(Vector2 pos, Vector2 size) {
	return (struct AABB2D){.center = {pos.x+size.x/2, pos.y+size.y/2}, .halfSize = {size.x/2, size.y/2}};
}

struct AABB3D aabb3D(Vector3 pos, Vector3 size) {
	return (struct AABB3D){.center = {pos.x+size.x/2, pos.y+size.y/2, pos.z+size.z}, .halfSize = {size.x/2, size.y/2, size.z/2}};
}

Vector2 vector2Value(float v) {
	return Vector2Scale(Vector2One(), v);
}

Vector3 vector3Value(float v) {
	return Vector3Scale(Vector3One(), v);
}

Vector4 vector3To4(Vector3 v) {
	return (Vector4){v.x, v.y, v.z, 1.0f};
}

Vector3 vector4To3(Vector4 v) {
	return (Vector3){v.x, v.y, v.z};
}

struct IVector2 vector2Int(Vector2 vec) {
	return (struct IVector2){
		(int)vec.x,
		(int)vec.y
	};
}

bool vector3NotZeroBasis(Vector3 v, Vector3 scale, float epsilon) {
	Vector3 v2 = Vector3Multiply(v, scale);

	return ((v2.x <= -epsilon || v2.y <= -epsilon || v2.z <= -epsilon) || (v2.x >= epsilon || v2.y >= epsilon || v2.z >= epsilon));
}

float* getVector3AxisUnsafe(Vector3* vec, int axis) {
	return ((float*)vec+axis);
}

float getVector3AxisSafe(Vector3 vec, int axis) {
	float tmp[3] = {
		vec.x,
		vec.y,
		vec.z
	};

	if(axis >= 0 && axis <= 2) {
		return tmp[axis];
	} else {
		return 0.0f;
	}
}

float projectRadiusOBB(struct OBB box, Vector2 axis) {
	float c = cosf(box.rotation);
	float s = sinf(box.rotation);

	Vector2 right = (Vector2){c, s};
	Vector2 up = (Vector2){-s, c};

	return box.halfSize.x * fabsf(Vector2DotProduct(axis, right)) +
		box.halfSize.y * fabsf(Vector2DotProduct(axis, up));
}

float vector2CompProduct(Vector2 v) {
	return v.x * v.y;
}

float vector3CompProduct(Vector3 v) {
	return v.x * v.y * v.z;
}

float vector2Average(Vector2 v) {
	float sum = v.x + v.y;

	return sum / 2.0f;
}

float vector3Average(Vector3 v) {
	float sum = v.x + v.y + v.z;

	return sum / 3.0f;
}

float getYawFromDirection(Vector3 direction)
{
    return atan2f(direction.x, -direction.z);
}

float getPitchFromDirection(Vector3 direction)
{
    return asinf(direction.y);
}

Vector3 getRotationForward(float pitch, float yaw)
{
    return (Vector3){
        sinf(yaw) * cosf(pitch),
        -sinf(pitch),
        cosf(yaw) * cosf(pitch)
    };
}

void camera3DLookAt(Camera3D* cam, Vector3 forward) {
	cam->target = Vector3Add(
		cam->position,
		forward
	);
}

// (Vector3){pitch, yaw, roll}
Vector3 getRotationFromDirection(Vector3 direction) {
	return (Vector3){getPitchFromDirection(direction), getYawFromDirection(direction), 0.0f}; // no roll
}

Vector3 getCamera3DForward(Camera3D camera) {
	return Vector3Normalize(Vector3Subtract(camera.target, camera.position));
}

Vector3 getCamera3DRight(Camera3D camera) {
	return Vector3Normalize(Vector3CrossProduct(getCamera3DForward(camera), camera.up));
}

float getBillboardYaw(Camera3D camera, Vector3 subjectPosition) {
	Vector3 distance = Vector3Subtract(
		camera.position,
		subjectPosition
	);
	return atan2f(distance.x, distance.z);
}

Vector3 getCameraRoll(Camera3D camera, float rollAngle) {
	return Vector3RotateByAxisAngle(
		(Vector3){0.0f, 1.0f, 0.0f},
		getCamera3DForward(camera),
		rollAngle
	);
}

void camera3DRotateEuler(Camera3D* cam, Vector3 rotation) {
	float pitch = rotation.x;
	float yaw = rotation.y;
	float roll = rotation.z;

	camera3DLookAt(cam, getRotationForward(pitch, yaw));
	cam->up = getCameraRoll(*cam, roll);
}

void drawCamera3D(Vector3 position, Vector3 rotation, Vector3 scale, Color color, int drawLine) {
	rlPushMatrix();
		Quaternion rot = QuaternionFromEuler(rotation.x, -rotation.y, rotation.z);

		scale = Vector3Multiply((Vector3){0.6f, 0.7f, 1.0f}, scale);

		Matrix final = MatrixIdentity();
		final = MatrixMultiply(
			final,
			MatrixScale(scale.x, scale.y, scale.z)
		);
		final = MatrixMultiply(
			final,
			QuaternionToMatrix(rot)
		);
		final = MatrixMultiply(
			final,
			MatrixTranslate(position.x, position.y, position.z)
		);
		rlMultMatrixf(MatrixToFloat(final));

		DrawCubeWiresV(Vector3Zero(), Vector3One(), color);

		rlPushMatrix();
			Matrix cone = MatrixIdentity();
			cone = MatrixMultiply(
				cone,
				MatrixRotateY((3.0f * M_PI) / 2.0f) // M_PI / 4.0f original
			);
			cone = MatrixMultiply(
				cone,
				MatrixRotateX((3.0f * M_PI) / 2.0f) // M_PI / 2.0f original
			);

			rlMultMatrixf(MatrixToFloat(cone));
			DrawCylinderWires(Vector3Zero(), 1.25f * vector3Average(scale), 0.0f, vector3Average(scale), 4, color);
		rlPopMatrix();
	rlPopMatrix();

	if(drawLine) {
		DrawLine3D(position, Vector3Add(
			position,
			Vector3Scale(
				getRotationForward(rotation.x, rotation.y),
				2.0f
			)
		), WHITE);
	}
}

void drawCrosshair(Vector2 origin, int extent, Color color) {
	DrawLine((int)origin.x, (int)(extent + origin.y), (int)origin.x, (int)(extent + origin.y), color);
	DrawLine(extent + origin.x, origin.y, extent + origin.x, origin.y, color);
}

void drawAxes3D(float extent, float scale) {
	extent *= 2.5f;
	scale *= 0.23f;

	Vector3 basisVectors[3] = {
		(Vector3){1.0f, 0.0f, 0.0f},
		(Vector3){0.0f, 1.0f, 0.0f},
		(Vector3){0.0f, 0.0f, 1.0f}
	};

	Color basisColors[3] = {
		RED,
		GREEN,
		BLUE
	};

	Vector3 fullBasis;
	Vector3 zero = Vector3Zero();
	Vector3 boxScale = vector3Value(0.15f);

	for(int i = 0; i < 3; i++) {
		fullBasis = Vector3Multiply(vector3Value(extent), basisVectors[i]);

		Vector3 forward = Vector3Normalize(fullBasis);
		Vector3 ref = (Vector3){0,1,0};

		Vector3 axis = Vector3CrossProduct(ref, forward);
		float angle = acosf(Vector3DotProduct(ref, forward));

		if (Vector3Length(axis) < 0.0001f)
			axis = (Vector3){1,0,0};
		else
			axis = Vector3Normalize(axis);

		DrawLine3D(zero, fullBasis, basisColors[i]);

		rlPushMatrix();
		rlTranslatef(fullBasis.x, fullBasis.y, fullBasis.z);
		rlRotatef(angle * RAD2DEG, axis.x, axis.y, axis.z);

		DrawCylinder((Vector3){0,0,0}, 0.0f, scale*(1.0f/16.0f)*(extent/scale), scale*(1.0f/4.0f)*(extent/scale), 8, basisColors[i]);

		rlPopMatrix();

		// negative
		DrawLine3D(zero, Vector3Negate(fullBasis), basisColors[i]);
		DrawCubeV(Vector3Negate(fullBasis), Vector3Scale(boxScale, 0.4), basisColors[i]);
	}
}

Vector2 v2hermite(Vector2 p0, Vector2 m0, Vector2 p1, Vector2 m1, float t) {
	float t2 = t * t;
	float t3 = t2 * t;

	float h00 =  2.0f*t3 - 3.0f*t2 + 1.0f;
	float h10 =        t3 - 2.0f*t2 + t;
	float h01 = -2.0f*t3 + 3.0f*t2;
	float h11 =        t3 -      t2;

	Vector2 a = Vector2Scale(p0, h00);
	Vector2 b = Vector2Scale(m0, h10);
	Vector2 c = Vector2Scale(p1, h01);
	Vector2 d = Vector2Scale(m1, h11);

	return Vector2Add(Vector2Add(a, b), Vector2Add(c, d));
}

Vector3 v3hermite(Vector3 p0, Vector3 m0, Vector3 p1, Vector3 m1, float t) {
	float t2 = t * t;
	float t3 = t2 * t;

	float h00 =  2.0f*t3 - 3.0f*t2 + 1.0f;
	float h10 =        t3 - 2.0f*t2 + t;
	float h01 = -2.0f*t3 + 3.0f*t2;
	float h11 =        t3 -      t2;

	Vector3 a = Vector3Scale(p0, h00);
	Vector3 b = Vector3Scale(m0, h10);
	Vector3 c = Vector3Scale(p1, h01);
	Vector3 d = Vector3Scale(m1, h11);

	return Vector3Add(Vector3Add(a, b), Vector3Add(c, d));
}

void drawGrid2D(int x, int y, int width, int height, int spacing, int drawAxes, Color color) {
	DrawPixel(x, y, color);

	for(int i = 0; i < width + 1; i++) {
		DrawLine(x + (i * spacing), y, x + (i * spacing), y + height * spacing, color);
	}

	for(int j = 0; j < height + 1; j++) {
		DrawLine(x, y + (j * spacing), x + width * spacing, y + (j * spacing), color);
	}

	if(!(width%2) && !(height%2)) {
		if(drawAxes) {
			Vector2 center = {spacing * width / 2.0f, spacing * height / 2.0f};
			//DrawRectangle(center.x-5+x, center.y-5+y, 10, 10, MAROON);

			DrawLine(center.x-spacing+x, center.y+y, center.x+spacing+x, center.y+y, RED);
			DrawLine(center.x+x, center.y-spacing+y, center.x+x, center.y+spacing+y, GREEN);
		}
	}
}

int isVec2Normalized(Vector2 v) {
	return feqel(v.x*v.x + v.y*v.y, 1.0f);
}

int isVec3Normalized(Vector3 v) {
	return feqel(v.x*v.x + v.y*v.y + v.z*v.z, 1.0f);
}

float getRandomFloat(float min, float max) {
    float scale = (float)GetRandomValue(0, 1000000) / 1000000.0f; 
    return min + scale * (max - min);
}

Vector3 getRandomSphereDir() {
	float z = getRandomFloat(-1.0f, 1.0f);
	float th = getRandomFloat(0.0f, 2.0f * M_PI);
	float r = sqrtf(1.0f - z*z);

	return (Vector3){
		r * cosf(th),
		z,
		r * sinf(th)
	};
}

Vector3 getCamera3DEuler(Camera3D camera)
{
    Vector3 direction = Vector3Subtract(camera.target, camera.position);
    return getRotationFromDirection(direction);
}

float getCamera3DYaw(Camera3D camera)
{
    Vector3 direction = Vector3Subtract(camera.target, camera.position);
    return getYawFromDirection(direction);
}

float getCamera3DPitch(Camera3D camera)
{
    Vector3 direction = Vector3Subtract(camera.target, camera.position);
    return getPitchFromDirection(direction);
}

void camera3DAddYawPitch(Camera3D* camera, float yaw, float pitch)
{
    Vector3 rotation = getCamera3DEuler(*camera);

    rotation.y += yaw;
    rotation.x += pitch;

    camera->target = Vector3Add(
        camera->position,
        getRotationForward(rotation.x, rotation.y)
    );
}

void camera3DAddYaw(Camera3D* camera, float yaw)
{
    camera3DAddYawPitch(camera, yaw, 0.0f);
}

void camera3DAddPitch(Camera3D* camera, float pitch)
{
    camera3DAddYawPitch(camera, 0.0f, pitch);
}

void camera3DSetYawPitch(Camera3D* camera, float yaw, float pitch)
{
    camera->target = Vector3Add(
        camera->position,
        getRotationForward(pitch, yaw)
    );
}

void camera3DSetYaw(Camera3D* camera, float yaw)
{
    float pitch = getCamera3DPitch(*camera);
    camera3DSetYawPitch(camera, yaw, pitch);
}

void camera3DSetPitch(Camera3D* camera, float pitch)
{
    float yaw = getCamera3DYaw(*camera);
    camera3DSetYawPitch(camera, yaw, pitch);
}

void camera3DClampPitch(Camera3D* camera, float minPitch, float maxPitch)
{
    float yaw = getCamera3DYaw(*camera);
    float pitch = getCamera3DPitch(*camera);

    pitch = Clamp(pitch, minPitch, maxPitch);

    camera3DSetYawPitch(camera, yaw, pitch);
}

void camera3DClampYaw(Camera3D* camera, float minYaw, float maxYaw)
{
    float yaw = getCamera3DYaw(*camera);
    float pitch = getCamera3DPitch(*camera);

    yaw = Clamp(yaw, minYaw, maxYaw);

    camera3DSetYawPitch(camera, yaw, pitch);
}

void camera3DRotateYaw(Camera3D* camera, float amount)
{
    camera3DAddYaw(camera, amount);
}

void camera3DRotatePitch(Camera3D* camera, float amount)
{
    camera3DAddPitch(camera, amount);
}

void drawGraph(Vector3 origin) {

}
