// Devin Hill 2026

#pragma once

#include <raylib.h>
#include <stdlib.h>
#include "../util/Collision.hpp"

namespace lvk {

struct StaticAABBCollider {
	BoundingBox aabb;
	Vector3 scale;
	Vector3 translation;
};

enum ColliderType {
	CO_NONE = 0,
	CO_STATIC_AABB = 1,
	CO_STATIC_RTRI = 2
};

struct ColliderDescriptor {
	int type;
	void* data;
};

struct CollisionWorld {
public:
	int colliderCount;
	int colliderCapacity;
	struct ColliderDescriptor* colliders;
	//struct StaticHeapArena colliderStorage;

	int saabbColliderCount;
	int saabbColliderCapacity;
	int* saabbColliderIndices;

	int srtColliderCount;
	int srtColliderCapacity;
	int* srtColliderIndices;

	Matrix transform;

	CollisionWorld() {
		colliderCount = 0;
		colliderCapacity = 1;
		colliders = (ColliderDescriptor*)std::malloc(sizeof(struct ColliderDescriptor));
		//shArenaInit(&colliderStorage, 2UL<<21);
		saabbColliderCount = 0;
		saabbColliderCapacity = 1;
		saabbColliderIndices = (int*)std::malloc(sizeof(int));
		transform = MatrixIdentity();
	}

	void pushBackSAABB(struct StaticAABBCollider collider) {
		struct ColliderDescriptor descriptor;
		descriptor.type = CO_STATIC_AABB;
		//descriptor.data = shArenaAlloc(&colliderStorage, sizeof(struct StaticAABBCollider));
		*(struct StaticAABBCollider*)descriptor.data = collider;
		if(descriptor.data == NULL) {
			return;
		}

		if(colliderCount + 1 > colliderCapacity) {
			colliderCapacity *= 2;
			colliders = (ColliderDescriptor*)realloc(colliders, sizeof(struct ColliderDescriptor) * colliderCapacity);
		}
		colliders[colliderCount] = descriptor;
		colliderCount++;

		if(saabbColliderCount + 1 > saabbColliderCapacity) {
			saabbColliderCapacity *= 2;		
			saabbColliderIndices = (int*)realloc(saabbColliderIndices, sizeof(int) * saabbColliderCapacity);
		}
		saabbColliderIndices[saabbColliderCount] = colliderCount-1;
		saabbColliderCount++;
	}
};

struct StaticAABBCollider SAABBFromST(Vector3 scale, Vector3 translation) {
	struct StaticAABBCollider collider;
	collider.scale = scale;
	collider.translation = translation;
	collider.aabb = getSTAABB(collider.scale, collider.translation);

	return collider;
}

} // namespace lvk
