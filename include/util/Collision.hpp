// Devin Hill 2026

// TODO: fix

#pragma once

#include <float.h>
#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include "../geo/Triangle.hpp"

using namespace lvk::geo;

namespace lvk::util {

struct SphereTriangleCollision {
	Vec3 difference;
	bool hit;
	Vec3 normal;
	float penetration;
};

struct BoxTriangleCollision {
    Vec3 normal;
    float penetration;
    bool hit;
};

Vec3 getClosestPointTriangleVec(Vec3 p, Vec3 a, Vec3 b, Vec3 c) {
    Vec3 ab = b - a;
    Vec3 ac = c - a;
    Vec3 ap = p - a;

    float d1 = ab.dot(ap);
    float d2 = ac.dot(ap);

    // Vertex region A
    if (d1 <= 0.0f && d2 <= 0.0f)
        return a;

    Vec3 bp = p - b;
    float d3 = ab.dot(bp);
    float d4 = ac.dot(bp);

    // Vertex region B
    if (d3 >= 0.0f && d4 <= d3)
        return b;

    float vc = d1*d4 - d3*d2;

    // Edge region AB
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
    {
        float v = d1 / (d1 - d3);
        return a + (ab * v);
    }

    Vec3 cp = p - c;
    float d5 = ab.dot(cp);
    float d6 = ac.dot(cp);

    // Vertex region C
    if (d6 >= 0.0f && d5 <= d6)
        return c;

    float vb = d5*d2 - d1*d6;

    // Edge region AC
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
    {
        float w = d2 / (d2 - d6);
        return a + (ac * w);
    }

    float va = d3*d6 - d5*d4;

    // Edge region BC
    if (va <= 0.0f &&
        (d4 - d3) >= 0.0f &&
        (d5 - d6) >= 0.0f)
    {
        Vec3 bc = c - b;
        float w = (d4 - d3) /
                  ((d4 - d3) + (d5 - d6));

        return b + (bc * w);
    }

    // Face region
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;

    return a + (ab * v) + (ac * w);
}

Vec3 getClosestPointTriangle(Vec3 p, struct Triangle triangle) {
	return getClosestPointTriangleVec(p, triangle.a, triangle.b, triangle.c);
}

Vec3 getClosestPointAABB(Vec3 p, BoundingBox box) {
    return (Vec3){
        math::clamp<float>(p.x, box.min.x, box.max.x),
        math::clamp<float>(p.y, box.min.y, box.max.y),
        math::clamp<float>(p.z, box.min.z, box.max.z)
    };
}

Vec3 getClosestPointSegment(Vec3 p, Vec3 a, Vec3 b) {
    Vec3 ab = b - a;

    float abLenSq =
    ab.lengthSqr();

    if(abLenSq < 1e-8f)
        return a;

    float t = (p - a).dot(ab) / abLenSq;

    t = math::clamp<float>(t, 0.0f, 1.0f);

    return a + (ab * t);
}

void getClosestPointsSegments(Vec3 p1, Vec3 q1, Vec3 p2, Vec3 q2, Vec3 *c1, Vec3 *c2) {
    Vec3 d1 = q1 - p1;
    Vec3 d2 = q2 - p2;
    Vec3 r = p1 - p2;

    float a = d1.dot(d1);
    float e = d2.dot(d2);
    float f = d2.dot(r);

    float s;
    float t;

    if(a < 1e-8f)
    {
        s = 0.0f;
        t = math::clamp<float>(f/e, 0.0f, 1.0f);
    }
    else
    {
        float c = d1.dot(r);

        if(e < 1e-8f)
        {
            t = 0.0f;
            s = math::clamp<float>(-c/a, 0.0f, 1.0f);
        }
        else
        {
            float b = d1.dot(d2);
            float denom = a*e - b*b;

            if(fabsf(denom) > 1e-8f)
                s = math::clamp<float>((b*f - c*e)/denom, 0.0f, 1.0f);
            else
                s = 0.0f;

            t = (b*s + f)/e;

            if(t < 0.0f)
            {
                t = 0.0f;
                s = math::clamp<float>(-c/a, 0.0f, 1.0f);
            }
            else if(t > 1.0f)
            {
                t = 1.0f;
                s = math::clamp<float>((b - c)/a, 0.0f, 1.0f);
            }
        }
    }

    *c1 =
        p1 + (d1 * s);

    *c2 =
        p2 + (d2 * t);
}

// sphere triangle collision
struct SphereTriangleCollision getCollisionSphereTriangle(Vec3 sphereCenter, float radius, struct Triangle triangle) {
	struct SphereTriangleCollision stc = (struct SphereTriangleCollision){0};

	Vec3 closest = getClosestPointTriangle(
		sphereCenter,
		triangle
	);

	stc.difference =
		sphereCenter - closest;

	stc.hit =
		stc.difference.lengthSqr() <= radius * radius;

	float distSq = stc.difference.lengthSqr();

	if(distSq > 1e-8f) {
		float dist = sqrtf(distSq);

		stc.normal = 
			stc.difference *
			1.0f / dist;

		stc.penetration = radius - dist;
	} else {
		Vec3 ab = triangle.b - triangle.a;
		Vec3 ac = triangle.c - triangle.a;

		stc.normal =
			Vec3Normalize(
				Vec3CrossProduct(ab, ac));

		stc.penetration = radius;
	}

	return stc;
}

void resolveCollisionSphereTriangle(Vec3 *position, Vec3 *velocity, Vec3 normal, float penetration) {
    *position = Vec3Add(
        *position,
        Vec3Scale(normal, penetration));

    float vn =
        Vec3DotProduct(*velocity, normal);

    if(vn < 0.0f)
    {
        *velocity = Vec3Subtract(
            *velocity,
            Vec3Scale(normal, vn));
    }
}

void collideMeshSphereTriangles(Mesh mesh, Matrix transform, float radius, Vec3* position, Vec3* velocity) {
    for(int i = 0; i < mesh.triangleCount; i++)
    {
		int i0, i1, i2;

		if(mesh.indices != NULL) {
			i0 = mesh.indices[i*3 + 0];
			i1 = mesh.indices[i*3 + 1];
			i2 = mesh.indices[i*3 + 2];
		} else {
			i0 = i*3 + 0;
			i1 = i*3 + 1;
			i2 = i*3 + 2;
		}

        Vec3 a = {
            mesh.vertices[i0*3+0],
            mesh.vertices[i0*3+1],
            mesh.vertices[i0*3+2]
        };

        Vec3 b = {
            mesh.vertices[i1*3+0],
            mesh.vertices[i1*3+1],
            mesh.vertices[i1*3+2]
        };

        Vec3 c = {
            mesh.vertices[i2*3+0],
            mesh.vertices[i2*3+1],
            mesh.vertices[i2*3+2]
        };

        a = Vec3Transform(a, transform);
        b = Vec3Transform(b, transform);
        c = Vec3Transform(c, transform);

		struct Triangle triangle = (struct Triangle){
			a,
			b,
			c
		};

        struct SphereTriangleCollision hit =
            getCollisionSphereTriangle(
                *position,
                radius,
                triangle);

        if(hit.hit)
        {
            resolveCollisionSphereTriangle(
                position,
                velocity,
                hit.normal,
                hit.penetration);
        }
    }
}

// aabb triangle collision
struct BoxTriangleCollision getCollisionAABBTriangle(BoundingBox box, struct Triangle triangle) {
    struct BoxTriangleCollision btc = {0};

    Vec3 center = {
        (box.min.x + box.max.x) * 0.5f,
        (box.min.y + box.max.y) * 0.5f,
        (box.min.z + box.max.z) * 0.5f
    };

    Vec3 closestTri =
        getClosestPointTriangle(
            center,
            triangle
        );

    Vec3 closestBox =
        getClosestPointAABB(
            closestTri,
            box
        );

    Vec3 diff =
        Vec3Subtract(
            closestBox,
            closestTri
        );

    float distSq =
        Vec3LengthSqr(diff);

    if(distSq > 1e-8f)
        return btc;

    btc.hit = true;

    Vec3 ab =
        Vec3Subtract(
            triangle.b,
            triangle.a
        );

    Vec3 ac =
        Vec3Subtract(
            triangle.c,
            triangle.a
        );

    btc.normal =
        Vec3Normalize(
            Vec3CrossProduct(ab, ac)
        );

    float px =
        fminf(
            box.max.x - closestTri.x,
            closestTri.x - box.min.x
        );

    float py =
        fminf(
            box.max.y - closestTri.y,
            closestTri.y - box.min.y
        );

    float pz =
        fminf(
            box.max.z - closestTri.z,
            closestTri.z - box.min.z
        );

    btc.penetration =
        fminf(
            px,
            fminf(py, pz)
        );

    return btc;
}

void collideMeshAABBTriangles(Mesh mesh, Matrix transform, BoundingBox box, Vec3 *position, Vec3 *velocity) {
    for(int i = 0; i < mesh.triangleCount; i++)
    {
        int i0, i1, i2;

        if(mesh.indices != NULL)
        {
            i0 = mesh.indices[i*3 + 0];
            i1 = mesh.indices[i*3 + 1];
            i2 = mesh.indices[i*3 + 2];
        }
        else
        {
            i0 = i*3 + 0;
            i1 = i*3 + 1;
            i2 = i*3 + 2;
        }

        Vec3 a = {
            mesh.vertices[i0*3 + 0],
            mesh.vertices[i0*3 + 1],
            mesh.vertices[i0*3 + 2]
        };

        Vec3 b = {
            mesh.vertices[i1*3 + 0],
            mesh.vertices[i1*3 + 1],
            mesh.vertices[i1*3 + 2]
        };

        Vec3 c = {
            mesh.vertices[i2*3 + 0],
            mesh.vertices[i2*3 + 1],
            mesh.vertices[i2*3 + 2]
        };

        a = Vec3Transform(a, transform);
        b = Vec3Transform(b, transform);
        c = Vec3Transform(c, transform);

        struct Triangle triangle = {
            a,
            b,
            c
        };

        struct BoxTriangleCollision hit =
            getCollisionAABBTriangle(
                box,
                triangle
            );

        if(hit.hit)
        {
            resolveCollisionSphereTriangle(
                position,
                velocity,
                hit.normal,
                hit.penetration
            );

            Vec3 correction =
                Vec3Scale(
                    hit.normal,
                    hit.penetration
                );

            box.min = Vec3Add(
                box.min,
                correction
            );

            box.max = Vec3Add(
                box.max,
                correction
            );
        }
    }
}

// capsule triangle collision
struct SphereTriangleCollision getCollisionCapsuleTriangleLQ(struct Capsule capsule, struct Triangle triangle) {
    struct SphereTriangleCollision ctc = {0};

    Vec3 triCenter =
        Vec3Scale(
            Vec3Add(
                Vec3Add(
                    triangle.a,
                    triangle.b
                ),
                triangle.c
            ),
            1.0f / 3.0f
        );

    Vec3 sphereCenter =
        getClosestPointSegment(
            triCenter,
            capsule.start,
            capsule.end
        );

    struct SphereTriangleCollision stc =
        getCollisionSphereTriangle(
            sphereCenter,
            capsule.radius,
            triangle
        );

    ctc.hit = stc.hit;
    ctc.normal = stc.normal;
    ctc.penetration = stc.penetration;
    ctc.difference = stc.difference;

    return ctc;
}

void collideMeshCapsuleTrianglesLQ(Mesh mesh, Matrix transform, struct Capsule capsule, Vec3 *position, Vec3 *velocity) {
    for(int i = 0; i < mesh.triangleCount; i++)
    {
        int i0, i1, i2;

        if(mesh.indices)
        {
            i0 = mesh.indices[i*3 + 0];
            i1 = mesh.indices[i*3 + 1];
            i2 = mesh.indices[i*3 + 2];
        }
        else
        {
            i0 = i*3 + 0;
            i1 = i*3 + 1;
            i2 = i*3 + 2;
        }

        Vec3 a = {
            mesh.vertices[i0*3+0],
            mesh.vertices[i0*3+1],
            mesh.vertices[i0*3+2]
        };

        Vec3 b = {
            mesh.vertices[i1*3+0],
            mesh.vertices[i1*3+1],
            mesh.vertices[i1*3+2]
        };

        Vec3 c = {
            mesh.vertices[i2*3+0],
            mesh.vertices[i2*3+1],
            mesh.vertices[i2*3+2]
        };

        a = Vec3Transform(a, transform);
        b = Vec3Transform(b, transform);
        c = Vec3Transform(c, transform);

        struct Triangle triangle = {
            a,
            b,
            c
        };

        struct SphereTriangleCollision hit =
            getCollisionCapsuleTriangleLQ(
                capsule,
                triangle
            );

        if(hit.hit)
        {
            resolveCollisionSphereTriangle(
                position,
                velocity,
                hit.normal,
                hit.penetration
            );

            Vec3 correction =
                Vec3Scale(
                    hit.normal,
                    hit.penetration
                );

            capsule.start =
                Vec3Add(
                    capsule.start,
                    correction
                );

            capsule.end =
                Vec3Add(
                    capsule.end,
                    correction
                );
        }
    }
}

struct SphereTriangleCollision getCollisionCapsuleTriangleHQ(struct Capsule capsule, struct Triangle triangle) {
    struct SphereTriangleCollision result = {0};

    Vec3 segA = capsule.start;
    Vec3 segB = capsule.end;

    Vec3 bestCapsule = {0};
    Vec3 bestTriangle = {0};

    float bestDistSq = FLT_MAX;

    Vec3 triClosest =
        getClosestPointTriangle(
            segA,
            triangle
        );

    float distSq =
        Vec3DistanceSqr(
            segA,
            triClosest
        );

    if(distSq < bestDistSq)
    {
        bestDistSq = distSq;
        bestCapsule = segA;
        bestTriangle = triClosest;
    }

    triClosest =
        getClosestPointTriangle(
            segB,
            triangle
        );

    distSq =
        Vec3DistanceSqr(
            segB,
            triClosest
        );

    if(distSq < bestDistSq)
    {
        bestDistSq = distSq;
        bestCapsule = segB;
        bestTriangle = triClosest;
    }

    Vec3 c1;
    Vec3 c2;

    getClosestPointsSegments(
        segA,
        segB,
        triangle.a,
        triangle.b,
        &c1,
        &c2
    );

    distSq = Vec3DistanceSqr(c1, c2);

    if(distSq < bestDistSq)
    {
        bestDistSq = distSq;
        bestCapsule = c1;
        bestTriangle = c2;
    }

    getClosestPointsSegments(
        segA,
        segB,
        triangle.b,
        triangle.c,
        &c1,
        &c2
    );

    distSq = Vec3DistanceSqr(c1, c2);

    if(distSq < bestDistSq)
    {
        bestDistSq = distSq;
        bestCapsule = c1;
        bestTriangle = c2;
    }

    getClosestPointsSegments(
        segA,
        segB,
        triangle.c,
        triangle.a,
        &c1,
        &c2
    );

    distSq = Vec3DistanceSqr(c1, c2);

    if(distSq < bestDistSq)
    {
        bestDistSq = distSq;
        bestCapsule = c1;
        bestTriangle = c2;
    }

    if(bestDistSq >
       capsule.radius * capsule.radius)
    {
        return result;
    }

    result.hit = true;

    result.difference =
        Vec3Subtract(
            bestCapsule,
            bestTriangle
        );

    float dist =
        sqrtf(bestDistSq);

    if(dist > 1e-6f)
    {
        result.normal =
            Vec3Scale(
                result.difference,
                1.0f/dist
            );

        result.penetration =
            capsule.radius - dist;
    }
    else
    {
        Vec3 ab =
            Vec3Subtract(
                triangle.b,
                triangle.a
            );

        Vec3 ac =
            Vec3Subtract(
                triangle.c,
                triangle.a
            );

        result.normal =
            Vec3Normalize(
                Vec3CrossProduct(
                    ab,
                    ac
                )
            );

        result.penetration =
            capsule.radius;
    }

    return result;
}

void collideMeshCapsuleTrianglesHQ(Mesh mesh, Matrix transform, struct Capsule capsule, Vec3 *position, Vec3 *velocity) {
    for(int i = 0; i < mesh.triangleCount; i++)
    {
        int i0, i1, i2;

        if(mesh.indices)
        {
            i0 = mesh.indices[i*3 + 0];
            i1 = mesh.indices[i*3 + 1];
            i2 = mesh.indices[i*3 + 2];
        }
        else
        {
            i0 = i*3 + 0;
            i1 = i*3 + 1;
            i2 = i*3 + 2;
        }

        Vec3 a = {
            mesh.vertices[i0*3+0],
            mesh.vertices[i0*3+1],
            mesh.vertices[i0*3+2]
        };

        Vec3 b = {
            mesh.vertices[i1*3+0],
            mesh.vertices[i1*3+1],
            mesh.vertices[i1*3+2]
        };

        Vec3 c = {
            mesh.vertices[i2*3+0],
            mesh.vertices[i2*3+1],
            mesh.vertices[i2*3+2]
        };

        a = Vec3Transform(a, transform);
        b = Vec3Transform(b, transform);
        c = Vec3Transform(c, transform);

        struct Triangle triangle = {
            a,
            b,
            c
        };

        struct SphereTriangleCollision hit =
            getCollisionCapsuleTriangleHQ(
                capsule,
                triangle
            );

        if(hit.hit)
        {
            resolveCollisionSphereTriangle(
                position,
                velocity,
                hit.normal,
                hit.penetration
            );

            Vec3 correction =
                Vec3Scale(
                    hit.normal,
                    hit.penetration
                );

            capsule.start =
                Vec3Add(
                    capsule.start,
                    correction
                );

            capsule.end =
                Vec3Add(
                    capsule.end,
                    correction
                );
        }
    }
}

BoundingBox getSTAABB(Vec3 scale, Vec3 translation) {
	Vec3 half = Vec3Scale(scale, 1/2.0f);

	BoundingBox box;
	box.min = Vec3Add(Vec3Scale(half, -1.0f), translation);
	box.max = Vec3Add(half, translation);

	return box;
}

Vec3 getSAABB(BoundingBox box) {
	return Vec3Subtract(box.max, box.min);
}
Vec3 getTAABB(BoundingBox box) {
	return Vec3Scale(
		Vec3Add(box.min, box.max),
		1.0f/2.0f
	);
}

Vec3 getOverlapAABB(BoundingBox testBox, BoundingBox worldBox) {
	return (Vec3){
		fminf(testBox.max.x, worldBox.max.x) -
		fmaxf(testBox.min.x, worldBox.min.x),
		fminf(testBox.max.y, worldBox.max.y) -
		fmaxf(testBox.min.y, worldBox.min.y),
		fminf(testBox.max.z, worldBox.max.z) -
		fmaxf(testBox.min.z, worldBox.min.z)
	};
}

} // namespace lvk::util
