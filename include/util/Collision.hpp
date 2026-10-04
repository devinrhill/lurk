// Devin Hill 2026

#pragma once

#include <float.h>
#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include "../geo/Triangle.hpp"

using namespace lvk::geo;

namespace lvk::util {

struct SphereTriangleCollision {
	Vector3 difference;
	bool hit;
	Vector3 normal;
	float penetration;
};

struct BoxTriangleCollision {
    Vector3 normal;
    float penetration;
    bool hit;
};

Vector3 getClosestPointTriangleVec(Vector3 p, Vector3 a, Vector3 b, Vector3 c) {
    Vector3 ab = Vector3Subtract(b, a);
    Vector3 ac = Vector3Subtract(c, a);
    Vector3 ap = Vector3Subtract(p, a);

    float d1 = Vector3DotProduct(ab, ap);
    float d2 = Vector3DotProduct(ac, ap);

    // Vertex region A
    if (d1 <= 0.0f && d2 <= 0.0f)
        return a;

    Vector3 bp = Vector3Subtract(p, b);
    float d3 = Vector3DotProduct(ab, bp);
    float d4 = Vector3DotProduct(ac, bp);

    // Vertex region B
    if (d3 >= 0.0f && d4 <= d3)
        return b;

    float vc = d1*d4 - d3*d2;

    // Edge region AB
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
    {
        float v = d1 / (d1 - d3);
        return Vector3Add(a, Vector3Scale(ab, v));
    }

    Vector3 cp = Vector3Subtract(p, c);
    float d5 = Vector3DotProduct(ab, cp);
    float d6 = Vector3DotProduct(ac, cp);

    // Vertex region C
    if (d6 >= 0.0f && d5 <= d6)
        return c;

    float vb = d5*d2 - d1*d6;

    // Edge region AC
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
    {
        float w = d2 / (d2 - d6);
        return Vector3Add(a, Vector3Scale(ac, w));
    }

    float va = d3*d6 - d5*d4;

    // Edge region BC
    if (va <= 0.0f &&
        (d4 - d3) >= 0.0f &&
        (d5 - d6) >= 0.0f)
    {
        Vector3 bc = Vector3Subtract(c, b);
        float w = (d4 - d3) /
                  ((d4 - d3) + (d5 - d6));

        return Vector3Add(b, Vector3Scale(bc, w));
    }

    // Face region
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;

    return Vector3Add(
        a,
        Vector3Add(
            Vector3Scale(ab, v),
            Vector3Scale(ac, w)
        )
    );
}

Vector3 getClosestPointTriangle(Vector3 p, struct Triangle triangle) {
	return getClosestPointTriangleVec(p, triangle.a, triangle.b, triangle.c);
}

Vector3 getClosestPointAABB(Vector3 p, BoundingBox box) {
    return (Vector3){
        Clamp(p.x, box.min.x, box.max.x),
        Clamp(p.y, box.min.y, box.max.y),
        Clamp(p.z, box.min.z, box.max.z)
    };
}

Vector3 getClosestPointSegment(Vector3 p, Vector3 a, Vector3 b) {
    Vector3 ab = Vector3Subtract(b, a);

    float abLenSq =
    Vector3LengthSqr(ab);

    if(abLenSq < 1e-8f)
        return a;

    float t =
    Vector3DotProduct(
        Vector3Subtract(p, a),
                      ab
    ) / abLenSq;

    t = Clamp(t, 0.0f, 1.0f);

    return Vector3Add(
        a,
        Vector3Scale(ab, t)
    );
}

void getClosestPointsSegments(Vector3 p1, Vector3 q1, Vector3 p2, Vector3 q2, Vector3 *c1, Vector3 *c2) {
    Vector3 d1 = Vector3Subtract(q1, p1);
    Vector3 d2 = Vector3Subtract(q2, p2);
    Vector3 r = Vector3Subtract(p1, p2);

    float a = Vector3DotProduct(d1, d1);
    float e = Vector3DotProduct(d2, d2);
    float f = Vector3DotProduct(d2, r);

    float s;
    float t;

    if(a < 1e-8f)
    {
        s = 0.0f;
        t = Clamp(f/e, 0.0f, 1.0f);
    }
    else
    {
        float c = Vector3DotProduct(d1, r);

        if(e < 1e-8f)
        {
            t = 0.0f;
            s = Clamp(-c/a, 0.0f, 1.0f);
        }
        else
        {
            float b = Vector3DotProduct(d1, d2);
            float denom = a*e - b*b;

            if(fabsf(denom) > 1e-8f)
                s = Clamp((b*f - c*e)/denom, 0.0f, 1.0f);
            else
                s = 0.0f;

            t = (b*s + f)/e;

            if(t < 0.0f)
            {
                t = 0.0f;
                s = Clamp(-c/a, 0.0f, 1.0f);
            }
            else if(t > 1.0f)
            {
                t = 1.0f;
                s = Clamp((b - c)/a, 0.0f, 1.0f);
            }
        }
    }

    *c1 =
    Vector3Add(
        p1,
        Vector3Scale(d1, s)
    );

    *c2 =
    Vector3Add(
        p2,
        Vector3Scale(d2, t)
    );
}

// sphere triangle collision
struct SphereTriangleCollision getCollisionSphereTriangle(Vector3 sphereCenter, float radius, struct Triangle triangle) {
	struct SphereTriangleCollision stc = (struct SphereTriangleCollision){0};

	Vector3 closest = getClosestPointTriangle(
		sphereCenter,
		triangle
	);

	stc.difference = Vector3Subtract(
		sphereCenter,
		closest
	);

	stc.hit =
		Vector3LengthSqr(stc.difference) <= radius * radius;

	float distSq = Vector3LengthSqr(stc.difference);

	if(distSq > 1e-8f) {
		float dist = sqrtf(distSq);

		stc.normal = Vector3Scale(
			stc.difference,
			1.0f / dist);

		stc.penetration = radius - dist;
	} else {
		Vector3 ab = Vector3Subtract(triangle.b, triangle.a);
		Vector3 ac = Vector3Subtract(triangle.c, triangle.a);

		stc.normal =
			Vector3Normalize(
				Vector3CrossProduct(ab, ac));

		stc.penetration = radius;
	}

	return stc;
}

void resolveCollisionSphereTriangle(Vector3 *position, Vector3 *velocity, Vector3 normal, float penetration) {
    *position = Vector3Add(
        *position,
        Vector3Scale(normal, penetration));

    float vn =
        Vector3DotProduct(*velocity, normal);

    if(vn < 0.0f)
    {
        *velocity = Vector3Subtract(
            *velocity,
            Vector3Scale(normal, vn));
    }
}

void collideMeshSphereTriangles(Mesh mesh, Matrix transform, float radius, Vector3* position, Vector3* velocity) {
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

        Vector3 a = {
            mesh.vertices[i0*3+0],
            mesh.vertices[i0*3+1],
            mesh.vertices[i0*3+2]
        };

        Vector3 b = {
            mesh.vertices[i1*3+0],
            mesh.vertices[i1*3+1],
            mesh.vertices[i1*3+2]
        };

        Vector3 c = {
            mesh.vertices[i2*3+0],
            mesh.vertices[i2*3+1],
            mesh.vertices[i2*3+2]
        };

        a = Vector3Transform(a, transform);
        b = Vector3Transform(b, transform);
        c = Vector3Transform(c, transform);

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

    Vector3 center = {
        (box.min.x + box.max.x) * 0.5f,
        (box.min.y + box.max.y) * 0.5f,
        (box.min.z + box.max.z) * 0.5f
    };

    Vector3 closestTri =
        getClosestPointTriangle(
            center,
            triangle
        );

    Vector3 closestBox =
        getClosestPointAABB(
            closestTri,
            box
        );

    Vector3 diff =
        Vector3Subtract(
            closestBox,
            closestTri
        );

    float distSq =
        Vector3LengthSqr(diff);

    if(distSq > 1e-8f)
        return btc;

    btc.hit = true;

    Vector3 ab =
        Vector3Subtract(
            triangle.b,
            triangle.a
        );

    Vector3 ac =
        Vector3Subtract(
            triangle.c,
            triangle.a
        );

    btc.normal =
        Vector3Normalize(
            Vector3CrossProduct(ab, ac)
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

void collideMeshAABBTriangles(Mesh mesh, Matrix transform, BoundingBox box, Vector3 *position, Vector3 *velocity) {
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

        Vector3 a = {
            mesh.vertices[i0*3 + 0],
            mesh.vertices[i0*3 + 1],
            mesh.vertices[i0*3 + 2]
        };

        Vector3 b = {
            mesh.vertices[i1*3 + 0],
            mesh.vertices[i1*3 + 1],
            mesh.vertices[i1*3 + 2]
        };

        Vector3 c = {
            mesh.vertices[i2*3 + 0],
            mesh.vertices[i2*3 + 1],
            mesh.vertices[i2*3 + 2]
        };

        a = Vector3Transform(a, transform);
        b = Vector3Transform(b, transform);
        c = Vector3Transform(c, transform);

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

            Vector3 correction =
                Vector3Scale(
                    hit.normal,
                    hit.penetration
                );

            box.min = Vector3Add(
                box.min,
                correction
            );

            box.max = Vector3Add(
                box.max,
                correction
            );
        }
    }
}

// capsule triangle collision
struct SphereTriangleCollision getCollisionCapsuleTriangleLQ(struct Capsule capsule, struct Triangle triangle) {
    struct SphereTriangleCollision ctc = {0};

    Vector3 triCenter =
        Vector3Scale(
            Vector3Add(
                Vector3Add(
                    triangle.a,
                    triangle.b
                ),
                triangle.c
            ),
            1.0f / 3.0f
        );

    Vector3 sphereCenter =
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

void collideMeshCapsuleTrianglesLQ(Mesh mesh, Matrix transform, struct Capsule capsule, Vector3 *position, Vector3 *velocity) {
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

        Vector3 a = {
            mesh.vertices[i0*3+0],
            mesh.vertices[i0*3+1],
            mesh.vertices[i0*3+2]
        };

        Vector3 b = {
            mesh.vertices[i1*3+0],
            mesh.vertices[i1*3+1],
            mesh.vertices[i1*3+2]
        };

        Vector3 c = {
            mesh.vertices[i2*3+0],
            mesh.vertices[i2*3+1],
            mesh.vertices[i2*3+2]
        };

        a = Vector3Transform(a, transform);
        b = Vector3Transform(b, transform);
        c = Vector3Transform(c, transform);

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

            Vector3 correction =
                Vector3Scale(
                    hit.normal,
                    hit.penetration
                );

            capsule.start =
                Vector3Add(
                    capsule.start,
                    correction
                );

            capsule.end =
                Vector3Add(
                    capsule.end,
                    correction
                );
        }
    }
}

struct SphereTriangleCollision getCollisionCapsuleTriangleHQ(struct Capsule capsule, struct Triangle triangle) {
    struct SphereTriangleCollision result = {0};

    Vector3 segA = capsule.start;
    Vector3 segB = capsule.end;

    Vector3 bestCapsule = {0};
    Vector3 bestTriangle = {0};

    float bestDistSq = FLT_MAX;

    Vector3 triClosest =
        getClosestPointTriangle(
            segA,
            triangle
        );

    float distSq =
        Vector3DistanceSqr(
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
        Vector3DistanceSqr(
            segB,
            triClosest
        );

    if(distSq < bestDistSq)
    {
        bestDistSq = distSq;
        bestCapsule = segB;
        bestTriangle = triClosest;
    }

    Vector3 c1;
    Vector3 c2;

    getClosestPointsSegments(
        segA,
        segB,
        triangle.a,
        triangle.b,
        &c1,
        &c2
    );

    distSq = Vector3DistanceSqr(c1, c2);

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

    distSq = Vector3DistanceSqr(c1, c2);

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

    distSq = Vector3DistanceSqr(c1, c2);

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
        Vector3Subtract(
            bestCapsule,
            bestTriangle
        );

    float dist =
        sqrtf(bestDistSq);

    if(dist > 1e-6f)
    {
        result.normal =
            Vector3Scale(
                result.difference,
                1.0f/dist
            );

        result.penetration =
            capsule.radius - dist;
    }
    else
    {
        Vector3 ab =
            Vector3Subtract(
                triangle.b,
                triangle.a
            );

        Vector3 ac =
            Vector3Subtract(
                triangle.c,
                triangle.a
            );

        result.normal =
            Vector3Normalize(
                Vector3CrossProduct(
                    ab,
                    ac
                )
            );

        result.penetration =
            capsule.radius;
    }

    return result;
}

void collideMeshCapsuleTrianglesHQ(Mesh mesh, Matrix transform, struct Capsule capsule, Vector3 *position, Vector3 *velocity) {
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

        Vector3 a = {
            mesh.vertices[i0*3+0],
            mesh.vertices[i0*3+1],
            mesh.vertices[i0*3+2]
        };

        Vector3 b = {
            mesh.vertices[i1*3+0],
            mesh.vertices[i1*3+1],
            mesh.vertices[i1*3+2]
        };

        Vector3 c = {
            mesh.vertices[i2*3+0],
            mesh.vertices[i2*3+1],
            mesh.vertices[i2*3+2]
        };

        a = Vector3Transform(a, transform);
        b = Vector3Transform(b, transform);
        c = Vector3Transform(c, transform);

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

            Vector3 correction =
                Vector3Scale(
                    hit.normal,
                    hit.penetration
                );

            capsule.start =
                Vector3Add(
                    capsule.start,
                    correction
                );

            capsule.end =
                Vector3Add(
                    capsule.end,
                    correction
                );
        }
    }
}

BoundingBox getSTAABB(Vector3 scale, Vector3 translation) {
	Vector3 half = Vector3Scale(scale, 1/2.0f);

	BoundingBox box;
	box.min = Vector3Add(Vector3Scale(half, -1.0f), translation);
	box.max = Vector3Add(half, translation);

	return box;
}

Vector3 getSAABB(BoundingBox box) {
	return Vector3Subtract(box.max, box.min);
}
Vector3 getTAABB(BoundingBox box) {
	return Vector3Scale(
		Vector3Add(box.min, box.max),
		1.0f/2.0f
	);
}

Vector3 getOverlapAABB(BoundingBox testBox, BoundingBox worldBox) {
	return (Vector3){
		fminf(testBox.max.x, worldBox.max.x) -
		fmaxf(testBox.min.x, worldBox.min.x),
		fminf(testBox.max.y, worldBox.max.y) -
		fmaxf(testBox.min.y, worldBox.min.y),
		fminf(testBox.max.z, worldBox.max.z) -
		fmaxf(testBox.min.z, worldBox.min.z)
	};
}

} // namespace lvk::util
