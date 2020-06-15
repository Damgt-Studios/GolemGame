#pragma once
#include "DirectXMath.h"
#include "ADMath.h"

namespace ADPhysics
{
	//Forward
	struct AABB;
	struct OBB;
	struct Sphere;
	struct Plane;
	struct Triangle;
	struct Manifold;

	static bool SphereToAabbCollision(const Sphere& sphere, const AABB& aabb, Manifold& m);
	static bool AabbToAabbCollision(const AABB& box1, const AABB& box2, Manifold& m);
	static bool AabbToObbCollision(const AABB& aabb, const OBB& obb, Manifold& m);
	static bool AabbToPlaneCollision(const AABB& aabb, const Plane& plane, Manifold& m);
	static bool SphereToPlaneCollision(const Sphere& sphere, const Plane& plane, Manifold& m);
	static bool ObbToPlaneCollision(const OBB& obb, const Plane& plane, Manifold& m);
	static bool SphereToObbCollision(const Sphere& sphere, const OBB& obb, Manifold& m);
	static bool ObbToObbCollision(const OBB& box1, const OBB& box2, Manifold& m);
	static bool TriangleToSphereCollision(const Triangle& tri, const Sphere& sphere, Manifold& m);
	static bool TriangleToAabbCollision(const Triangle& tri, const AABB& aabb, Manifold& m);
	static bool TriangleToObbCollision(const Triangle& tri, const OBB& obb, Manifold& m);


	enum class ColliderType
	{
		Sphere, Aabb, Obb, Plane, Triangle
	};

	struct Collider 
	{
		XMFLOAT3 Pos;
		ColliderType type;
		bool trigger = false;

		virtual bool isCollision(Sphere* other, Manifold& m) {
			return false;
		}

		virtual bool isCollision(AABB* other, Manifold& m) {
			return false;
		}

		virtual bool isCollision(OBB* other, Manifold& m) {
			return false;
		}

		virtual bool isCollision(Plane* other, Manifold& m) {
			return false;
		}

		virtual float GetWidth() {
			return 0;
		}

		virtual float GetHeight() {
			return 0;
		}

		virtual float GetLength() {
			return 0;
		}

	};

	struct AABB : Collider
	{
		XMFLOAT3 Min, Max, Extents, HalfSize;
		AABB()
		{
			Pos = XMFLOAT3(0, 0, 0);
			HalfSize = XMFLOAT3(0.5f, 0.5f, 0.5f);
			Max = XMFLOAT3(Pos.x + HalfSize.x, Pos.y + HalfSize.y, Pos.z + HalfSize.z);
			Min = XMFLOAT3(Pos.x - HalfSize.x, Pos.y - HalfSize.y, Pos.z - HalfSize.z);
			Extents = XMFLOAT3(Max.x - Pos.x, Max.y - Pos.y, Max.z - Pos.z);
			type = ColliderType::Aabb;
		};
		AABB(XMFLOAT3 Position, XMFLOAT3 Size)
		{
			Pos = Position;
			HalfSize = VectorToFloat3(Float3ToVector(Size) / 2);
			Max = XMFLOAT3(Pos.x + HalfSize.x, Pos.y + HalfSize.y, Pos.z + HalfSize.z);
			Min = XMFLOAT3(Pos.x - HalfSize.x, Pos.y - HalfSize.y, Pos.z - HalfSize.z);
			Extents = XMFLOAT3(Max.x - Pos.x, Max.y - Pos.y, Max.z - Pos.z);
			type = ColliderType::Aabb;
		};

		virtual bool isCollision(Sphere* other, Manifold& m) {
			return SphereToAabbCollision(*other, *this, m);
		}

		virtual bool isCollision(AABB* other, Manifold& m) {
			return AabbToAabbCollision(*this, *other, m);
		}

		virtual bool isCollision(OBB* other, Manifold& m) {
			return AabbToObbCollision(*this, *other, m);
		}

		virtual bool isCollision(Plane* other, Manifold& m) {
			return AabbToPlaneCollision(*this, *other, m);
		}

		virtual float GetWidth() {
			return HalfSize.x * 2;
		}

		virtual float GetHeight() {
			return HalfSize.y * 2;
		}

		virtual float GetLength() {
			return HalfSize.z * 2;
		}
	};

	struct OBB : Collider
	{
		XMFLOAT3 AxisX, AxisY, AxisZ, HalfSize;
		OBB()
		{
			Pos = XMFLOAT3(0, 0, 0);
			AxisX = XMFLOAT3(1, 0, 0);
			AxisY = XMFLOAT3(0, 1, 0);
			AxisZ = XMFLOAT3(0, 0, 1);
			HalfSize = XMFLOAT3(0.5f, 0.5f, 0.5f);
			type = ColliderType::Obb;
		};
		OBB(XMMATRIX Transform, XMFLOAT3 Size)
		{
			Pos = (XMFLOAT3&)Transform.r[3];
			AxisX = (XMFLOAT3&)Transform.r[0];
			AxisY = (XMFLOAT3&)Transform.r[1];
			AxisZ = (XMFLOAT3&)Transform.r[2];
			HalfSize = VectorToFloat3(Float3ToVector(Size) / 2);
			type = ColliderType::Obb;
		}

		virtual bool isCollision(Sphere* other, Manifold& m) {
			return SphereToObbCollision(*other, *this, m);
		}

		virtual bool isCollision(AABB* other, Manifold& m) {
			return AabbToObbCollision(*other, *this, m);
		}

		virtual bool isCollision(OBB* other, Manifold& m) {
			return ObbToObbCollision(*this, *other, m);
		}

		virtual bool isCollision(Plane* other, Manifold& m) {
			return ObbToPlaneCollision(*this, *other, m);
		}

		virtual float GetWidth() {
			return HalfSize.x * 2;
		}

		virtual float GetHeight() {
			return HalfSize.y * 2;
		}

		virtual float GetLength() {
			return HalfSize.z * 2;
		}

	};

	struct Sphere : Collider
	{
		float Radius;
		Sphere()
		{
			Pos = XMFLOAT3(0, 0, 0);
			Radius = 0.5f;
			type = ColliderType::Sphere;
		}

		Sphere(XMFLOAT3 Position, float Radius) : Radius(Radius) { Pos = Position; type = ColliderType::Sphere; };

		virtual float GetWidth() {
			return Radius;
		}

		virtual float GetHeight() {
			return Radius;
		}

		virtual float GetLength() {
			return Radius;
		}
	};

	struct Plane : Collider
	{
		XMFLOAT3 Normal, AxisX, AxisY, AxisZ, HalfSize;
		Plane()
		{
			Pos = XMFLOAT3(0, 0, 0);
			Normal = XMFLOAT3(0, 1, 0);
			AxisX = XMFLOAT3(1, 0, 0);
			AxisY = Normal;
			AxisZ = XMFLOAT3(0, 0, 1);
			HalfSize = XMFLOAT3(0.5f, 0.5f, 0.5f);
			type = ColliderType::Plane;
		}

		Plane(XMMATRIX Transform, XMFLOAT3 Size)
		{
			Pos = (XMFLOAT3&)Transform.r[3];
			AxisX = (XMFLOAT3&)Transform.r[0];
			AxisY = (XMFLOAT3&)Transform.r[1];
			AxisZ = (XMFLOAT3&)Transform.r[2];
			Normal = AxisY;
			HalfSize = VectorToFloat3(Float3ToVector(Size) / 2);
			type = ColliderType::Plane;
		}

		virtual bool isCollision(Sphere* other, Manifold& m) {
			return SphereToPlaneCollision(*other, *this, m);
		}

		virtual bool isCollision(AABB* other, Manifold& m) {
			return AabbToPlaneCollision(*other, *this, m);
		}

		virtual bool isCollision(OBB* other, Manifold& m) {
			return ObbToPlaneCollision(*other, *this, m);
		}

		virtual float GetWidth() {
			return HalfSize.x * 2;
		}

		virtual float GetHeight() {
			return HalfSize.y * 2;
		}

		virtual float GetLength() {
			return HalfSize.z * 2;
		}
		
	};

	struct InfinitePlane
	{
		XMFLOAT3 normal;
		float distance;
	};

	struct Ray {
		XMFLOAT3 Origin, Direction;

		Ray(XMFLOAT3 origin, XMFLOAT3 direction) : Origin(origin), Direction(direction) {};
	};

	struct Segment
	{
		XMFLOAT3 Start, End;

		Segment(XMFLOAT3 s, XMFLOAT3 e) : Start(s), End(e) {};
	};

	struct Triangle : Collider
	{
		XMFLOAT3 a;
		XMFLOAT3 b;
		XMFLOAT3 c;

		Triangle(XMFLOAT3 A, XMFLOAT3 B, XMFLOAT3 C) : a(A), b(B), c(C) 
		{ Pos = (XMFLOAT3&)((Float3ToVector(A) + Float3ToVector(B) + Float3ToVector(C)) / 3); type = ColliderType::Triangle; };

		virtual bool isCollision(Sphere* other, Manifold& m) {
			return TriangleToSphereCollision(*this, *other, m);
		}

		virtual bool isCollision(AABB* other, Manifold& m) {
			return TriangleToAabbCollision(*this, *other, m);
		}

		virtual bool isCollision(OBB* other, Manifold& m) {
			return TriangleToObbCollision(*this, *other, m);
		}

		virtual float GetWidth() {
			float min = this->a.x < this->b.x ? this->a.x : this->b.x; min = min < this->c.x ? min : this->c.x;
			float max = this->a.x > this->b.x ? this->a.x : this->b.x; max = max > this->c.x ? max : this->c.x;
			return max - min;
		}

		virtual float GetHeight() {
			float min = this->a.y < this->b.y ? this->a.y : this->b.y; min = min < this->c.y ? min : this->c.y;
			float max = this->a.y > this->b.y ? this->a.y : this->b.y; max = max > this->c.y ? max : this->c.y;
			return max - min;
		}

		virtual float GetLength() {
			float min = this->a.z < this->b.z ? this->a.z : this->b.z; min = min < this->c.z ? min : this->c.z;
			float max = this->a.z > this->b.z ? this->a.z : this->b.z; max = max > this->c.z ? max : this->c.z;
			return max - min;
		}
	};

	struct PhysicsMaterial
	{
		float Mass;
		float InvMass;
		float Restitution;

		PhysicsMaterial() : Mass(1), InvMass(1), Restitution(0.5f) {};
		PhysicsMaterial(float Mass, float InvMass, float Rest) : Mass(Mass), InvMass(InvMass), Restitution(Rest) {};
	};

	struct Manifold
	{
		float PenetrationDepth;
		XMFLOAT3 Normal;
		XMFLOAT4 ContactPoint;
	};

	struct Interval
	{
		float min, max;
	};

	static float Gravity = -9.8f;
	static float maxDownwardVelocity = -20.0f;

	//------------------------------------------Seperating Axes
	static bool SerparatingAxisTest(const XMFLOAT3& Difference, const XMFLOAT3& Axis, const OBB& box1, const OBB& box2)
	{
		return (fabs(VectorDot(Difference, Axis)) >
			(fabs(VectorDot((Float3Multiply(box1.AxisX, box1.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(box1.AxisY, box1.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(box1.AxisZ, box1.HalfSize.z)), Axis)) +
				fabs(VectorDot((Float3Multiply(box2.AxisX, box2.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(box2.AxisY, box2.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(box2.AxisZ, box2.HalfSize.z)), Axis))));
	}

	static bool SerparatingAxisTest(const XMFLOAT3& Difference, const XMFLOAT3& Axis, const Sphere& sphere, const Plane& plane)
	{
		return (fabs(VectorDot(Difference, Axis)) >
			(fabs(VectorDot((Float3Multiply(XMFLOAT3(1, 0, 0), sphere.Radius)), Axis)) +
				fabs(VectorDot((Float3Multiply(XMFLOAT3(0, 1, 0), sphere.Radius)), Axis)) +
				fabs(VectorDot((Float3Multiply(XMFLOAT3(0, 0, 1), sphere.Radius)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisX, plane.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisY, plane.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisZ, plane.HalfSize.z)), Axis))));
	}

	static bool SerparatingAxisTest(const XMFLOAT3& Difference, const XMFLOAT3& Axis, const AABB& box1, const Plane& plane)
	{
		return (fabs(VectorDot(Difference, Axis)) >
			(fabs(VectorDot((Float3Multiply(XMFLOAT3(1, 0, 0), box1.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(XMFLOAT3(0, 1, 0), box1.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(XMFLOAT3(0, 0, 1), box1.HalfSize.z)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisX, plane.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisY, plane.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisZ, plane.HalfSize.z)), Axis))));
	}

	static bool SerparatingAxisTest(const XMFLOAT3& Difference, const XMFLOAT3& Axis, const OBB& box1, const Plane& plane)
	{
		return (fabs(VectorDot(Difference, Axis)) >
			(fabs(VectorDot((Float3Multiply(box1.AxisX, box1.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(box1.AxisY, box1.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(box1.AxisZ, box1.HalfSize.z)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisX, plane.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisY, plane.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisZ, plane.HalfSize.z)), Axis))));
	}

	static bool SerparatingAxisTest(const XMFLOAT3& Difference, const XMFLOAT3& Axis, const AABB& aabb, const OBB& obb)
	{
		return (fabs(VectorDot(Difference, Axis)) >
			(fabs(VectorDot((Float3Multiply(XMFLOAT3(1, 0, 0), aabb.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(XMFLOAT3(0, 1, 0), aabb.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(XMFLOAT3(0, 0, 1), aabb.HalfSize.z)), Axis)) +
				fabs(VectorDot((Float3Multiply(obb.AxisX, obb.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(obb.AxisY, obb.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(obb.AxisZ, obb.HalfSize.z)), Axis))));
	}

	static bool SerparatingAxisTest(const XMFLOAT3& Difference, const XMFLOAT3& Axis, const AABB& aabb, const Triangle& tri)
	{
		return (fabs(VectorDot(Difference, Axis)) >
			(fabs(VectorDot((Float3Multiply(XMFLOAT3(1, 0, 0), aabb.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(XMFLOAT3(0, 1, 0), aabb.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(XMFLOAT3(0, 0, 1), aabb.HalfSize.z)), Axis)) +
				fabs(VectorDot((XMFLOAT3&)((XMVECTOR&)tri.b - (XMVECTOR&)tri.a), Axis)) +
				fabs(VectorDot((XMFLOAT3&)((XMVECTOR&)tri.a - (XMVECTOR&)tri.c), Axis)) +
				fabs(VectorDot((XMFLOAT3&)((XMVECTOR&)tri.c - (XMVECTOR&)tri.b), Axis))));
	}

	//------------------------------------------Helper Function

	static float CalculateDistance(const XMFLOAT3& first, const XMFLOAT3& second)
	{
		return sqrt(pow(second.x - first.x, 2) + pow(second.y - first.y, 2) + pow(second.z - first.z, 2));
	}

	static float CalculateSquaredDistance(const XMFLOAT3& first, const XMFLOAT3& second)
	{
		return pow(second.x - first.x, 2) + pow(second.y - first.y, 2) + pow(second.z - first.z, 2);
	}

	static float Magnitude(const XMFLOAT3& Vector)
	{
		return sqrt(pow(Vector.x, 2) + pow(Vector.y, 2) + pow(Vector.z, 2));
	}

	static float MagnitudeSq(const XMFLOAT3& Vector)
	{
		return pow(Vector.x, 2) + pow(Vector.y, 2) + pow(Vector.z, 2);
	}

	static XMFLOAT3 Project(const XMFLOAT3& length, const XMFLOAT3& direction)
	{
		float dot = VectorDot(length, direction);
		float magSq = MagnitudeSq(direction);
		return (XMFLOAT3&)(Float3ToVector(direction) * (dot / magSq));
	}

	static XMFLOAT3 Perpendicular(const XMFLOAT3& length, const XMFLOAT3& direction) 
	{
		return (XMFLOAT3&)(Float3ToVector(length) - Float3ToVector(Project(length, direction)));
	}

	static XMFLOAT3 FindBarycentric(XMFLOAT3& Point, const Triangle& tri)
	{
		XMVECTOR ap = Float3ToVector(Point) - Float3ToVector(tri.a);
		XMVECTOR bp = Float3ToVector(Point) - Float3ToVector(tri.b);
		XMVECTOR cp = Float3ToVector(Point) - Float3ToVector(tri.c);

		XMVECTOR ab = Float3ToVector(tri.b) - Float3ToVector(tri.a);
		XMVECTOR ac = Float3ToVector(tri.c) - Float3ToVector(tri.a);
		XMVECTOR bc = Float3ToVector(tri.c) - Float3ToVector(tri.b);
		XMVECTOR cb = Float3ToVector(tri.b) - Float3ToVector(tri.c);
		XMVECTOR ca = Float3ToVector(tri.a) - Float3ToVector(tri.c);

		XMVECTOR v = ab - Float3ToVector(Project((XMFLOAT3&)ab, (XMFLOAT3&)cb));
		float a = 1.0f - (VectorDot(v, ap) / VectorDot(v, ab));

		v = bc - Float3ToVector(Project((XMFLOAT3&)bc, (XMFLOAT3&)ac));
		float b = 1.0f - (VectorDot(v, bp) / VectorDot(v, bc));

		v = ca - Float3ToVector(Project((XMFLOAT3&)ca, (XMFLOAT3&)ab));
		float c = 1.0f - (VectorDot(v, cp) / VectorDot(v, ca));

		return XMFLOAT3(a, b, c);
	}

	static InfinitePlane FromTriangle(const Triangle& t)
	{
		InfinitePlane result;
		result.normal = (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(t.b) - Float3ToVector(t.a), Float3ToVector(t.c) - Float3ToVector(t.a)));
		result.distance = VectorDot(result.normal, t.a);
		return result;
	};

	static bool PointToTriangle(const XMFLOAT3& Point, const Triangle& tri)
	{
		XMVECTOR a = (XMVECTOR&)tri.a - (XMVECTOR&)Point;
		XMVECTOR b = (XMVECTOR&)tri.b - (XMVECTOR&)Point;
		XMVECTOR c = (XMVECTOR&)tri.c - (XMVECTOR&)Point;

		XMVECTOR normPBC = XMVector3Normalize(XMVector3Cross(b, c)); // Normal of PBC (u)
		XMVECTOR normPCA = XMVector3Normalize(XMVector3Cross(c, a)); // Normal of PCA (v)
		XMVECTOR normPAB = XMVector3Normalize(XMVector3Cross(a, b)); // Normal of PAB (w)

		float w1 = VectorDot(normPBC, normPCA), w2 = VectorDot(normPBC, normPAB);
		if (w1 <= 0.0f) {
			return false;
		}
		else if (w2 <= 0.0f) {
			return false;
		}
		else if (w1 + w2 > 1.0f)

		return true;
	}

	static XMFLOAT3 FindClosestPoint(const XMFLOAT3& Point, const Segment& line) 
	{
		XMVECTOR vec = (XMVECTOR&)line.End - (XMVECTOR&)line.Start;
		float length = Magnitude((XMFLOAT3&)vec);
		vec = XMVector3Normalize(vec);
		XMVECTOR temp = (XMVECTOR&)Point - (XMVECTOR&)line.Start;
		float dot = VectorDot(vec, temp);
		XMFLOAT3 ClosestPoint;
		if (dot < 0)
			ClosestPoint = line.Start;
		else if (dot > length)
			ClosestPoint = line.End;
		else
		{
			XMVECTOR Nprime = vec * dot;
			ClosestPoint = (XMFLOAT3&)((XMVECTOR&)line.Start + Nprime);
		}
		return ClosestPoint;
	}

	static XMFLOAT3 FindClosestPoint(const XMFLOAT3& Point, const Triangle& tri) 
	{
		auto ClosestPointPlane = [](const XMFLOAT3& point, const InfinitePlane plane) -> XMFLOAT3
		{
			float dot = VectorDot(plane.normal, point);
			float distance = dot - plane.distance;
			return (XMFLOAT3&)((XMVECTOR&)point - (XMVECTOR&)plane.normal * distance);
		};

		InfinitePlane plane = FromTriangle(tri);
		XMFLOAT3 Closest = ClosestPointPlane(Point, plane);
		if (PointToTriangle(Closest, tri))
			return Closest;

		XMFLOAT3 c1 = FindClosestPoint(Point, Segment(tri.a, tri.b)); // Line AB
		XMFLOAT3 c2 = FindClosestPoint(Point, Segment(tri.b, tri.c)); // Line BC
		XMFLOAT3 c3 = FindClosestPoint(Point, Segment(tri.c, tri.a)); // Line CA

		float magSq1 = MagnitudeSq((XMFLOAT3&)((XMVECTOR&)Point - (XMVECTOR&)c1));
		float magSq2 = MagnitudeSq((XMFLOAT3&)((XMVECTOR&)Point - (XMVECTOR&)c2));
		float magSq3 = MagnitudeSq((XMFLOAT3&)((XMVECTOR&)Point - (XMVECTOR&)c3));

		if (magSq1 <= magSq2 && magSq1 <= magSq3) {
			return c1;
		}
		else if (magSq2 <= magSq1 && magSq2 <= magSq3) {
			return c2;
		}

		return c3;
	}

	static XMFLOAT3 FindClosestPoint(const XMFLOAT3& Point, const Sphere& sphere)
	{
		XMVECTOR SphereToPoint = (XMVECTOR&)Point - (XMVECTOR&)sphere.Pos;
		SphereToPoint = XMVector3Normalize(SphereToPoint);
		SphereToPoint *= sphere.Radius;
		return (XMFLOAT3&)(SphereToPoint + (XMVECTOR&)sphere.Pos);
	}

	static XMFLOAT3 FindClosestPoint(const XMFLOAT3& Point, const AABB& aabb)
	{
		XMFLOAT3 result = Point;
		XMFLOAT3 max = aabb.Max;
		XMFLOAT3 min = aabb.Min;

		result.x = result.x < min.x ? min.x : result.x;
		result.y = result.y < min.y ? min.y : result.y;
		result.z = result.z < min.z ? min.z : result.z;

		result.x = result.x > max.x ? max.x : result.x;
		result.y = result.y > max.y ? max.y : result.y;
		result.z = result.z > max.z ? max.z : result.z;

		return result;
	}

	static XMFLOAT3 FindClosestPoint(const XMFLOAT3& Point, const OBB& obb)
	{
		XMFLOAT3 result = obb.Pos;
		XMFLOAT3 Direction = (XMFLOAT3&)((XMVECTOR&)Point - (XMVECTOR&)result);

		float distance = VectorDot(Direction, obb.AxisX);

		if (distance > obb.HalfSize.x)
			distance = obb.HalfSize.x;

		if (distance < -obb.HalfSize.x)
			distance = -obb.HalfSize.x;

		result = (XMFLOAT3&)((XMVECTOR&)result + ((XMVECTOR&)obb.AxisX * distance));

		distance = VectorDot(Direction, obb.AxisY);

		if (distance > obb.HalfSize.y)
			distance = obb.HalfSize.y;

		if (distance < -obb.HalfSize.y)
			distance = -obb.HalfSize.y;

		result = (XMFLOAT3&)((XMVECTOR&)result + ((XMVECTOR&)obb.AxisY * distance));

		distance = VectorDot(Direction, obb.AxisZ);

		if (distance > obb.HalfSize.z)
			distance = obb.HalfSize.z;

		if (distance < -obb.HalfSize.z)
			distance = -obb.HalfSize.z;

		result = (XMFLOAT3&)((XMVECTOR&)result + ((XMVECTOR&)obb.AxisZ * distance));

		return result;
	}

	static Interval GetInterval(const AABB& aabb, const XMFLOAT3& axis)
	{
		Interval result;

		XMFLOAT3 a = aabb.Min;
		XMFLOAT3 i = aabb.Max;

		XMFLOAT3 vertex[] =
		{
			XMFLOAT3(i.x, a.y, a.z),
			XMFLOAT3(i.x, a.y, i.z),
			XMFLOAT3(i.x, i.y, a.z),
			XMFLOAT3(i.x, i.y, i.z),
			XMFLOAT3(a.x, a.y, a.z),
			XMFLOAT3(a.x, a.y, i.z),
			XMFLOAT3(a.x, i.y, a.z),
			XMFLOAT3(a.x, i.y, i.z)
		};

		result.min = result.max = VectorDot(axis, vertex[0]);

		for (unsigned int i = 1; i < 8; i++)
		{
			float projection = VectorDot(axis, vertex[i]);
			result.min = projection < result.min ? projection : result.min;
			result.max = projection > result.max ? projection : result.max;
		}
		return result;
	}

	static Interval GetInterval(const OBB& obb, const XMFLOAT3& axis)
	{
		Interval result;

		XMFLOAT3 vertex[8];

		XMVECTOR C = (XMVECTOR&)obb.Pos;

		vertex[0] = (XMFLOAT3&)(C + Float3ToVector(obb.AxisX) * obb.HalfSize.x + Float3ToVector(obb.AxisY) * obb.HalfSize.y + Float3ToVector(obb.AxisZ) * obb.HalfSize.z);
		vertex[1] = (XMFLOAT3&)(C - Float3ToVector(obb.AxisX) * obb.HalfSize.x + Float3ToVector(obb.AxisY) * obb.HalfSize.y + Float3ToVector(obb.AxisZ) * obb.HalfSize.z);
		vertex[2] = (XMFLOAT3&)(C + Float3ToVector(obb.AxisX) * obb.HalfSize.x - Float3ToVector(obb.AxisY) * obb.HalfSize.y + Float3ToVector(obb.AxisZ) * obb.HalfSize.z);
		vertex[3] = (XMFLOAT3&)(C + Float3ToVector(obb.AxisX) * obb.HalfSize.x + Float3ToVector(obb.AxisY) * obb.HalfSize.y - Float3ToVector(obb.AxisZ) * obb.HalfSize.z);
		vertex[4] = (XMFLOAT3&)(C - Float3ToVector(obb.AxisX) * obb.HalfSize.x - Float3ToVector(obb.AxisY) * obb.HalfSize.y - Float3ToVector(obb.AxisZ) * obb.HalfSize.z);
		vertex[5] = (XMFLOAT3&)(C + Float3ToVector(obb.AxisX) * obb.HalfSize.x - Float3ToVector(obb.AxisY) * obb.HalfSize.y - Float3ToVector(obb.AxisZ) * obb.HalfSize.z);
		vertex[6] = (XMFLOAT3&)(C - Float3ToVector(obb.AxisX) * obb.HalfSize.x + Float3ToVector(obb.AxisY) * obb.HalfSize.y - Float3ToVector(obb.AxisZ) * obb.HalfSize.z);
		vertex[7] = (XMFLOAT3&)(C - Float3ToVector(obb.AxisX) * obb.HalfSize.x - Float3ToVector(obb.AxisY) * obb.HalfSize.y + Float3ToVector(obb.AxisZ) * obb.HalfSize.z);

		result.min = result.max = VectorDot(axis, vertex[0]);

		for (unsigned int i = 1; i < 8; i++)
		{
			float projection = VectorDot(axis, vertex[i]);
			result.min = projection < result.min ? projection : result.min;
			result.max = projection > result.max ? projection : result.max;
		}
		return result;
	}

	static Interval GetInterval(const Triangle& tri, const XMFLOAT3& axis)
	{
		Interval result;
		result.min = result.max = VectorDot(axis, tri.c);

		float value = VectorDot(axis, tri.b);
		result.min = fminf(result.min, value);
		result.max = fmaxf(result.max, value);

		value = VectorDot(axis, tri.a);
		result.min = fminf(result.min, value);
		result.max = fmaxf(result.max, value);

		return result;
	}

	static float RaycastToPlane(const Ray& r, const InfinitePlane& inf)
	{
		float nd = VectorDot(r.Direction, inf.normal);
		float np = VectorDot(r.Origin, inf.normal);

		if (nd >= 0.0f)
			return -1;

		float t = (inf.distance - np) / nd;

		if (t >= 0.0f)
			return t;

		return -1;
	}

	static float RaycastToTriangle(const Ray& r, const Triangle& tri, Manifold& m)
	{
		InfinitePlane plane = FromTriangle(tri);
		float t = RaycastToPlane(r, plane);
		if (t < 0.0f) {
			return t;
		}

		XMFLOAT3 result = (XMFLOAT3&)(Float3ToVector(r.Origin) + Float3ToVector(r.Direction) * t);

		XMFLOAT3 barycentric = FindBarycentric(result, tri);
		if (barycentric.x >= 0.0f && barycentric.x <= 1.0f &&
			barycentric.y >= 0.0f && barycentric.y <= 1.0f &&
			barycentric.z >= 0.0f && barycentric.z <= 1.0f) 
		{
			m.ContactPoint = XMFLOAT4(result.x, result.y, result.z, 1);
			return t;
		}

		return -1;
	}

	static bool LineSegmentToTriangle(const Segment& line, const Triangle& tri, Manifold& m)
	{
		Ray r = Ray(line.Start, (XMFLOAT3&)XMVector3Normalize(Float3ToVector(line.End) - Float3ToVector(line.Start)));
		float t = RaycastToTriangle(r, tri, m);

		bool temp = t >= 0 && t * t <= MagnitudeSq((XMFLOAT3&)(Float3ToVector(line.End) - Float3ToVector(line.Start)));

		if (temp == false)
			return false;

		return true;
	}

	//------------------------------------------Self Collision

	static bool SphereToSphereCollision(const Sphere& sphere1, const Sphere& sphere2, Manifold& m)
	{
		float Distance = CalculateDistance(sphere1.Pos, sphere2.Pos);
		float Radii = sphere1.Radius + sphere2.Radius;

		if (Distance > Radii)
			return false;

		m.Normal = (XMFLOAT3&)((XMVECTOR&)sphere2.Pos - (XMVECTOR&)sphere1.Pos);
		m.PenetrationDepth = Radii - Distance;

		return true;
	}

	static bool AabbToAabbCollision(const AABB& box1, const AABB& box2, Manifold& m)
	{
		if (box1.Max.x < box2.Min.x || box1.Min.x > box2.Max.x) return false;
		if (box1.Max.y < box2.Min.y || box1.Min.y > box2.Max.y) return false;
		if (box1.Max.z < box2.Min.z || box1.Min.z > box2.Max.z) return false;

		XMFLOAT3 collisionMax, collisionMin;
		collisionMax.x = min(box1.Max.x, box2.Max.x);
		collisionMax.y = min(box1.Max.y, box2.Max.y);
		collisionMax.z = min(box1.Max.z, box2.Max.z);

		collisionMin.x = max(box1.Min.x, box2.Min.x);
		collisionMin.y = max(box1.Min.y, box2.Min.y);
		collisionMin.z = max(box1.Min.z, box2.Min.z);

		XMFLOAT4 Overlap = VectorToFloat4((XMVECTOR&)collisionMax - (XMVECTOR&)collisionMin);
		XMFLOAT3 ContactNormal = XMFLOAT3();
		float PenetrationDepth = 0;

		if (Overlap.x <= Overlap.y && Overlap.x <= Overlap.z)
		{
			ContactNormal = XMFLOAT3(1, 0, 0);
			PenetrationDepth = Overlap.x;
		}
		if (Overlap.y <= Overlap.x && Overlap.y <= Overlap.z)
		{
			ContactNormal = XMFLOAT3(0, 1, 0);
			PenetrationDepth = Overlap.y;
		}
		if (Overlap.z <= Overlap.x && Overlap.z <= Overlap.y)
		{
			ContactNormal = XMFLOAT3(0, 0, 1);
			PenetrationDepth = Overlap.z;
		}

		XMVECTOR Vector = (XMVECTOR&)(box2.Pos) - (XMVECTOR&)(box1.Pos);
		XMFLOAT4 FloatVector = VectorToFloat4(Vector);

		float dot = FloatVector.x * ContactNormal.x + FloatVector.y * ContactNormal.y + FloatVector.z * ContactNormal.z;

		if (dot < 0)
			ContactNormal = XMFLOAT3(-ContactNormal.x, -ContactNormal.y, -ContactNormal.z);

		XMVECTOR cp = ((XMVECTOR&)collisionMax + (XMVECTOR&)collisionMin) * 0.5f;
		cp -= (XMVECTOR&)ContactNormal * PenetrationDepth * 0.5f;

		//Contact Point can be found and used to call Event System at this position
		XMFLOAT4 ContactPoint = VectorToFloat4(cp);

		m.ContactPoint = ContactPoint;
		m.Normal = ContactNormal;
		m.PenetrationDepth = PenetrationDepth;

		return true;
	}

	static bool ObbToObbCollision(const OBB& box1, const OBB& box2, Manifold& m)
	{
		static XMFLOAT3 Difference;
		Difference = XMFLOAT3(box2.Pos.x - box1.Pos.x, box2.Pos.y - box1.Pos.y, box2.Pos.z - box1.Pos.z);

		bool temp = !(
			ADPhysics::SerparatingAxisTest(Difference, box1.AxisX, box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, box1.AxisY, box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, box1.AxisZ, box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, box2.AxisX, box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, box2.AxisY, box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, box2.AxisZ, box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisX), Float3ToVector(box2.AxisX)), box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisX), Float3ToVector(box2.AxisY)), box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisX), Float3ToVector(box2.AxisZ)), box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisY), Float3ToVector(box2.AxisX)), box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisY), Float3ToVector(box2.AxisY)), box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisY), Float3ToVector(box2.AxisZ)), box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisZ), Float3ToVector(box2.AxisX)), box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisZ), Float3ToVector(box2.AxisY)), box1, box2) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisZ), Float3ToVector(box2.AxisZ)), box1, box2));

		if (temp == false)
			return false;

		//Axises Needed are required to be Normalized. First 6 should already be normalized
		XMFLOAT3 Axises[] = {
			 box1.AxisX,
			 box1.AxisY,
			 box1.AxisZ,
			 box2.AxisX,
			 box2.AxisY,
			 box2.AxisZ,
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(box1.AxisX), Float3ToVector(box2.AxisX))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(box1.AxisX), Float3ToVector(box2.AxisY))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(box1.AxisX), Float3ToVector(box2.AxisZ))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(box1.AxisY), Float3ToVector(box2.AxisX))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(box1.AxisY), Float3ToVector(box2.AxisY))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(box1.AxisY), Float3ToVector(box2.AxisZ))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(box1.AxisZ), Float3ToVector(box2.AxisX))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(box1.AxisZ), Float3ToVector(box2.AxisY))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(Float3ToVector(box1.AxisZ), Float3ToVector(box2.AxisZ)))
		};

		int AxisOfMinimumCollision;
		float MinOverlap = 100;

		//Floating Point "Fix"
		for (unsigned int i = 0; i < 15; i++)
		{
			if (Axises[i].x < 0.00001f && Axises[i].x > -0.00001f)
				Axises[i].x = 0;

			if (Axises[i].y < 0.00001f && Axises[i].y > -0.00001f)
				Axises[i].y = 0;

			if (Axises[i].z < 0.00001f && Axises[i].z > -0.00001f)
				Axises[i].z = 0;
		}

		//Get the Interval of each Axis
		for (unsigned int i = 0; i < 15; i++)
		{
			Interval a = GetInterval(box1, Axises[i]);
			Interval b = GetInterval(box2, Axises[i]);

			//If Seperates
			if ((b.min <= a.max) && (a.min <= b.max))
			{
				//Grab Smallest Overlap of this Axis
				float minBetween = min(b.max - a.min, a.max - b.min);
				//Overlaps Can't be Zero
				if (minBetween > 0)
				{
					if (MinOverlap > minBetween)
					{
						MinOverlap = minBetween;
						AxisOfMinimumCollision = i;
					}
				}
			}

		}

		//Manifold Setup
		m.Normal = Axises[AxisOfMinimumCollision];

		if (VectorDot((XMVECTOR&)box2.Pos - (XMVECTOR&)box1.Pos, (XMVECTOR&)m.Normal) < 0)
			m.Normal = XMFLOAT3(-m.Normal.x, -m.Normal.y, -m.Normal.z);

		m.PenetrationDepth = MinOverlap;

		return true;
	}

	//------------------------------------------Plane Collision

	static bool SphereToPlaneCollision(const Sphere& sphere, const Plane& plane, Manifold& m)
	{
		static XMFLOAT3 Difference;
		Difference = XMFLOAT3(plane.Pos.x - sphere.Pos.x, plane.Pos.y - sphere.Pos.y, plane.Pos.z - sphere.Pos.z);

		bool temp = !(
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(1, 0, 0), sphere, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 1, 0), sphere, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 0, 1), sphere, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisX, sphere, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisY, sphere, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisZ, sphere, plane));

		if (temp == false)
			return temp;

		XMVECTOR NormalPosition = XMVectorSet(plane.Pos.x + plane.Normal.x, plane.Pos.y + plane.Normal.y, plane.Pos.z + plane.Normal.z, 1);
		XMVECTOR L = XMVector3Normalize(NormalPosition - (XMVECTOR&)plane.Pos);
		XMVECTOR V = (XMVECTOR&)sphere.Pos - (XMVECTOR&)plane.Pos;

		float Dot = VectorDot(L, V);
		XMFLOAT4 ClosestPoint = (XMFLOAT4&)((XMVECTOR&)plane.Pos + (L * Dot));
		float Distance = CalculateDistance((XMFLOAT3&)ClosestPoint, plane.Pos);
		float overlap = sphere.Radius - Distance;

		m.Normal = plane.Normal;
		m.PenetrationDepth = overlap;

		return true;
	}

	static bool AabbToPlaneCollision(const AABB& box, const Plane& plane, Manifold& m)
	{
		static XMFLOAT3 Difference;
		Difference = XMFLOAT3(box.Pos.x - plane.Pos.x, box.Pos.y - plane.Pos.y, box.Pos.z - plane.Pos.z);

		bool temp = !(
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(1, 0, 0), box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 1, 0), box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 0, 1), box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisX, box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisY, box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisZ, box, plane));

		if (!temp)
			return temp;

		XMVECTOR NormalPosition = XMVectorSet(plane.Pos.x + plane.Normal.x, plane.Pos.y + plane.Normal.y, plane.Pos.z + plane.Normal.z, 1);
		XMVECTOR L = XMVector3Normalize(NormalPosition - (XMVECTOR&)plane.Pos);
		//Get Closest Point from AABB to line
		XMVECTOR V = (XMVECTOR&)box.Pos - (XMVECTOR&)plane.Pos;
		float Dot = VectorDot(L, V);
		XMFLOAT4 ClosestPoint = (XMFLOAT4&)((XMVECTOR&)plane.Pos + (L * Dot));
		float Distance = ADPhysics::CalculateDistance((XMFLOAT3&)ClosestPoint, plane.Pos);
		float radius = (abs(plane.Normal.x) * box.Extents.x) + (abs(plane.Normal.y) * box.Extents.y) + (abs(plane.Normal.z) * box.Extents.z);
		float overlap = radius - Distance;


		m.Normal = plane.Normal;
		m.PenetrationDepth = overlap;

		return true;
	}

	static bool ObbToPlaneCollision(const OBB& obb, const Plane& plane, Manifold& m)
	{
		static XMFLOAT3 Difference;
		Difference = XMFLOAT3(plane.Pos.x - obb.Pos.x, plane.Pos.y - obb.Pos.y, plane.Pos.z - obb.Pos.z);

		bool temp = !(
			ADPhysics::SerparatingAxisTest(Difference, obb.AxisX, obb, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, obb.AxisY, obb, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, obb.AxisZ, obb, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisX, obb, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisY, obb, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisZ, obb, plane));

		if (temp == false)
			return temp;

		XMVECTOR NormalPosition = XMVectorSet(plane.Pos.x + plane.Normal.x, plane.Pos.y + plane.Normal.y, plane.Pos.z + plane.Normal.z, 1);
		XMVECTOR L = XMVector3Normalize(NormalPosition - (XMVECTOR&)plane.Pos);
		XMVECTOR V = (XMVECTOR&)obb.Pos - (XMVECTOR&)plane.Pos;
		float Dot = VectorDot(L, V);
		XMFLOAT4 ClosestPoint = (XMFLOAT4&)((XMVECTOR&)plane.Pos + (L * Dot));
		XMFLOAT3 tempX = obb.AxisX;
		XMFLOAT3 tempY = obb.AxisY;
		XMFLOAT3 tempZ = obb.AxisZ;
		float testDot;
		testDot = VectorDot(plane.Normal, tempX);
		if (testDot < 0)
			tempX = XMFLOAT3(-tempX.x, -tempX.y, -tempX.z);
		testDot = VectorDot(plane.Normal, tempY);
		if (testDot < 0)
			tempY = XMFLOAT3(-tempY.x, -tempY.y, -tempY.z);
		testDot = VectorDot(plane.Normal, tempZ);
		if (testDot < 0)
			tempZ = XMFLOAT3(-tempZ.x, -tempZ.y, -tempZ.z);

		XMFLOAT3 tempF = (XMFLOAT3&)((Float3ToVector(tempX) + Float3ToVector(tempY) + Float3ToVector(tempZ)) * Float3ToVector(obb.HalfSize));
		float Distance = ADPhysics::CalculateDistance((XMFLOAT3&)ClosestPoint, plane.Pos);
		float radius = (abs(plane.Normal.x) * abs(tempF.x)) + (abs(plane.Normal.y) * abs(tempF.y)) + (abs(plane.Normal.z) * abs(tempF.z));
		float overlap = radius - Distance;

		m.Normal = plane.Normal;
		m.PenetrationDepth = overlap;

		return true;
	}

	//------------------------------------------Triangular Collisions


	static bool TriangleToSphereCollision(const Triangle& tri, const Sphere& sphere, Manifold& m)
	{
		XMFLOAT3 Closest = FindClosestPoint(sphere.Pos, tri);

		float MagSq = MagnitudeSq((XMFLOAT3&)((XMVECTOR&)Closest - (XMVECTOR&)sphere.Pos));
		bool temp = MagSq <= sphere.Radius * sphere.Radius;

		if (temp == false)
			return temp;

		m.Normal = (XMFLOAT3&)XMVector3Normalize(XMVector3Cross((XMVECTOR&)tri.b - (XMVECTOR&)tri.a, (XMVECTOR&)tri.c - (XMVECTOR&)tri.a));

		m.PenetrationDepth = sphere.Radius - Magnitude((XMFLOAT3&)((XMVECTOR&)Closest - (XMVECTOR&)sphere.Pos));

		return true;
	}

	static bool TriangleToAabbCollision(const Triangle& tri, const AABB& aabb, Manifold& m)
	{
		XMVECTOR f0 = (XMVECTOR&)tri.b - (XMVECTOR&)tri.a;
		XMVECTOR f1 = (XMVECTOR&)tri.c - (XMVECTOR&)tri.b;
		XMVECTOR f2 = (XMVECTOR&)tri.a - (XMVECTOR&)tri.c;

		XMVECTOR u0 = XMVectorSet(1, 0, 0, 1);
		XMVECTOR u1 = XMVectorSet(0, 1, 0, 1);
		XMVECTOR u2 = XMVectorSet(0, 0, 1, 1);

		XMFLOAT3 Axises[13] =
		{
			(XMFLOAT3&)u0, (XMFLOAT3&)u1, (XMFLOAT3&)u2,
			(XMFLOAT3&)XMVector3Normalize(XMVector3Cross(f0, f1)), 
			(XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u0, f0)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u0, f1)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u0, f2)),
			(XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u1, f0)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u1, f1)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u1, f2)),
			(XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u2, f0)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u2, f1)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u2, f2))
		};

		auto OverlappingAxis = [&](const Triangle& tri, const AABB& aabb, const XMFLOAT3& axis) -> bool
		{
			Interval a = GetInterval(tri, axis);
			Interval b = GetInterval(aabb, axis);
			return ((b.min <= a.max) && (a.min <= b.max));
		};

		for (int i = 0; i < 13; ++i) {
			if (!OverlappingAxis(tri, aabb, Axises[i]))
				return false; // Separating axis found
		}

		int AxisOfMinimumCollision;
		float MinOverlap = 100;

		//Floating Point "Fix"
		for (unsigned int i = 0; i < 15; i++)
		{
			if (Axises[i].x < 0.00001f && Axises[i].x > -0.00001f)
				Axises[i].x = 0;

			if (Axises[i].y < 0.00001f && Axises[i].y > -0.00001f)
				Axises[i].y = 0;

			if (Axises[i].z < 0.00001f && Axises[i].z > -0.00001f)
				Axises[i].z = 0;
		}

		//Get the Interval of each Axis
		for (unsigned int i = 0; i < 13; i++)
		{
			Interval a = GetInterval(tri, Axises[i]);
			Interval b = GetInterval(aabb, Axises[i]);

			//If Seperates
			if ((b.min <= a.max) && (a.min <= b.max))
			{
				//Grab Smallest Overlap of this Axis
				float minBetween = min(b.max - a.min, a.max - b.min);
				//Overlaps Can't be Zero
				if (minBetween > 0)
				{
					if (MinOverlap > minBetween)
					{
						MinOverlap = minBetween;
						AxisOfMinimumCollision = i;
					}
				}
			}

		}

		//Manifold Setup
		m.Normal = Axises[AxisOfMinimumCollision];

		XMFLOAT3 Closest = FindClosestPoint(aabb.Pos, tri);

		if (VectorDot((XMVECTOR&)aabb.Pos - (XMVECTOR&)Closest, (XMVECTOR&)m.Normal) <= 0)
			m.Normal = XMFLOAT3(-m.Normal.x, -m.Normal.y, -m.Normal.z);

		m.PenetrationDepth = MinOverlap;

		return true;
	}
	

	static bool TriangleToObbCollision(const Triangle& tri, const OBB& obb, Manifold& m)
	{
		XMVECTOR f0 = (XMVECTOR&)tri.b - (XMVECTOR&)tri.a;
		XMVECTOR f1 = (XMVECTOR&)tri.c - (XMVECTOR&)tri.b;
		XMVECTOR f2 = (XMVECTOR&)tri.a - (XMVECTOR&)tri.c;

		XMVECTOR u0 = (XMVECTOR&)obb.AxisX;
		XMVECTOR u1 = (XMVECTOR&)obb.AxisY;
		XMVECTOR u2 = (XMVECTOR&)obb.AxisZ;

		XMFLOAT3 Axises[13] =
		{
			(XMFLOAT3&)u0, (XMFLOAT3&)u1, (XMFLOAT3&)u2,
			(XMFLOAT3&)XMVector3Normalize(XMVector3Cross(f0, f1)),
			(XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u0, f0)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u0, f1)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u0, f2)),
			(XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u1, f0)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u1, f1)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u1, f2)),
			(XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u2, f0)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u2, f1)), (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(u2, f2))
		};

		auto OverlappingAxis = [&](const Triangle& tri, const OBB& obb, const XMFLOAT3& axis) -> bool
		{
			Interval a = GetInterval(tri, axis);
			Interval b = GetInterval(obb, axis);
			return ((b.min <= a.max) && (a.min <= b.max));
		};

		for (int i = 0; i < 13; ++i) {
			if (!OverlappingAxis(tri, obb, Axises[i]))
				return false; // Separating axis found
		}

		int AxisOfMinimumCollision;
		float MinOverlap = 100;

		//Floating Point "Fix"
		for (unsigned int i = 0; i < 15; i++)
		{
			if (Axises[i].x < 0.00001f && Axises[i].x > -0.00001f)
				Axises[i].x = 0;

			if (Axises[i].y < 0.00001f && Axises[i].y > -0.00001f)
				Axises[i].y = 0;

			if (Axises[i].z < 0.00001f && Axises[i].z > -0.00001f)
				Axises[i].z = 0;
		}

		//Get the Interval of each Axis
		for (unsigned int i = 0; i < 13; i++)
		{
			Interval a = GetInterval(tri, Axises[i]);
			Interval b = GetInterval(obb, Axises[i]);

			//If Seperates
			if ((b.min <= a.max) && (a.min <= b.max))
			{
				//Grab Smallest Overlap of this Axis
				float minBetween = min(b.max - a.min, a.max - b.min);
				//Overlaps Can't be Zero
				if (minBetween > 0)
				{
					if (MinOverlap > minBetween)
					{
						MinOverlap = minBetween;
						AxisOfMinimumCollision = i;
					}
				}
			}

		}

		//Manifold Setup
		m.Normal = Axises[AxisOfMinimumCollision];

		XMFLOAT3 Closest = FindClosestPoint(obb.Pos, tri);

		if (VectorDot((XMVECTOR&)obb.Pos - (XMVECTOR&)Closest, (XMVECTOR&)m.Normal) <= 0)
			m.Normal = XMFLOAT3(-m.Normal.x, -m.Normal.y, -m.Normal.z);

		m.PenetrationDepth = MinOverlap;

		return true;
	}

	//------------------------------------------Miscellaneous Collisions

	static bool SphereToAabbCollision(const Sphere& sphere, const AABB& aabb, Manifold& m)
	{
		XMFLOAT3 point = FindClosestPoint(sphere.Pos, aabb);
		float MagSq = MagnitudeSq((XMFLOAT3&)((XMVECTOR&)sphere.Pos - (XMVECTOR&)point));
		float RadSq = sphere.Radius * sphere.Radius;

		bool temp = MagSq < RadSq;

		if (temp == false)
			return temp;

		//Axis relative to the sphere
		m.Normal = (XMFLOAT3&)-XMVector3Normalize(((XMVECTOR&)sphere.Pos - (XMVECTOR&)point));

		float DistFromClosestToCenter = CalculateSquaredDistance(point, sphere.Pos);

		m.PenetrationDepth = sphere.Radius * sphere.Radius - DistFromClosestToCenter;

		return true;

	}

	static bool SphereToObbCollision(const Sphere& sphere, const OBB& obb, Manifold& m)
	{
		XMFLOAT3 point = FindClosestPoint(sphere.Pos, obb);
		float MagSq = MagnitudeSq((XMFLOAT3&)((XMVECTOR&)sphere.Pos - (XMVECTOR&)point));
		float RadSq = sphere.Radius * sphere.Radius;

		bool temp = MagSq < RadSq;

		if (temp == false)
			return temp;

		//Axis relative to the sphere
		m.Normal = (XMFLOAT3&)-((XMVECTOR&)sphere.Pos - (XMVECTOR&)point);

		//Floating Point "Fix"
		if (m.Normal.x < 0.00001f && m.Normal.x > -0.00001f)
			m.Normal.x = 0;

		if (m.Normal.y < 0.00001f && m.Normal.y > -0.00001f)
			m.Normal.y = 0;

		if (m.Normal.z < 0.00001f && m.Normal.z > -0.00001f)
			m.Normal.z = 0;

		float DistFromClosestToCenter = CalculateSquaredDistance(point, sphere.Pos);

		m.PenetrationDepth = sphere.Radius * sphere.Radius - DistFromClosestToCenter;

		return true;
	}

	static bool AabbToObbCollision(const AABB& aabb, const OBB& obb, Manifold& m)
	{
		static XMFLOAT3 Difference;
		Difference = XMFLOAT3(obb.Pos.x - aabb.Pos.x, obb.Pos.y - aabb.Pos.y, obb.Pos.z - aabb.Pos.z);

		bool temp = !(
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(1, 0, 0), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 1, 0), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 0, 1), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, obb.AxisX, aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, obb.AxisY, aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, obb.AxisZ, aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(1, 0, 0, 1), Float3ToVector(obb.AxisX)), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(1, 0, 0, 1), Float3ToVector(obb.AxisY)), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(1, 0, 0, 1), Float3ToVector(obb.AxisZ)), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 1, 0, 1), Float3ToVector(obb.AxisX)), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 1, 0, 1), Float3ToVector(obb.AxisY)), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 1, 0, 1), Float3ToVector(obb.AxisZ)), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 0, 1, 1), Float3ToVector(obb.AxisX)), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 0, 1, 1), Float3ToVector(obb.AxisY)), aabb, obb) ||
			ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 0, 1, 1), Float3ToVector(obb.AxisZ)), aabb, obb));

		if (temp == false)
			return false;

		//Axises Needed are required to be Normalized. First 6 should already be normalized
		XMFLOAT3 Axises[] = {
			 XMFLOAT3(1,0,0),
			 XMFLOAT3(0,1,0),
			 XMFLOAT3(0,0,1),
			 obb.AxisX,
			 obb.AxisY,
			 obb.AxisZ,
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(XMVectorSet(1, 0, 0, 1), Float3ToVector(obb.AxisX))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(XMVectorSet(1, 0, 0, 1), Float3ToVector(obb.AxisY))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(XMVectorSet(1, 0, 0, 1), Float3ToVector(obb.AxisZ))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 1), Float3ToVector(obb.AxisX))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 1), Float3ToVector(obb.AxisY))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 1), Float3ToVector(obb.AxisZ))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 0, 1, 1), Float3ToVector(obb.AxisX))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 0, 1, 1), Float3ToVector(obb.AxisY))),
			 (XMFLOAT3&)XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 0, 1, 1), Float3ToVector(obb.AxisZ)))
		};

		int AxisOfMinimumCollision ;
		float MinOverlap = 100;

		//Floating Point "Fix"
		for (unsigned int i = 0; i < 15; i++)
		{
			if (Axises[i].x < 0.00001f && Axises[i].x > -0.00001f)
				Axises[i].x = 0;

			if (Axises[i].y < 0.00001f && Axises[i].y > -0.00001f)
				Axises[i].y = 0;

			if (Axises[i].z < 0.00001f && Axises[i].z > -0.00001f)
				Axises[i].z = 0;
		}

		//Get the Interval of each Axis
		for (unsigned int i = 0; i < 15; i++)
		{
			Interval a = GetInterval(aabb, Axises[i]);
			Interval b = GetInterval(obb, Axises[i]);

			//If Seperates
			if ((b.min <= a.max) && (a.min <= b.max))
			{
				//Grab Smallest Overlap of this Axis
				float minBetween = min(b.max - a.min, a.max - b.min);
				//Overlaps Can't be Zero
				if (minBetween > 0)
				{
					if (MinOverlap > minBetween)
					{
						MinOverlap = minBetween;
						AxisOfMinimumCollision = i;
					}
				}
			}

		}

		//Manifold Setup
		m.Normal = Axises[AxisOfMinimumCollision];

		if (VectorDot(Float3ToVector(obb.Pos) - Float3ToVector(aabb.Pos), Float3ToVector(m.Normal)) < 0)
			m.Normal = XMFLOAT3(-m.Normal.x, -m.Normal.y, -m.Normal.z);

		m.PenetrationDepth = MinOverlap;

		return true;
	}



	//New Impulse Resolution

	static void PositionalCorrection(XMFLOAT4& Position1, const PhysicsMaterial& mat1, XMFLOAT4& Position2, const PhysicsMaterial& mat2, Manifold& m)
	{
		const float percent = 1; // usually 20% to 80%
		const float slop = 0.001; // usually 0.01f to 0.1f;
		XMVECTOR correction = max(abs(m.PenetrationDepth) - slop, 0.0f) / (mat1.InvMass + mat2.InvMass) * percent * (XMVECTOR&)m.Normal;
		correction.m128_f32[3] = 0;
		Position1 = (XMFLOAT4&)((XMVECTOR&)Position1 - mat1.InvMass * correction);
		Position2 = (XMFLOAT4&)((XMVECTOR&)Position2 + mat2.InvMass * correction);
	}

	static void VelocityImpulse(XMFLOAT4& Velocity1, const PhysicsMaterial& mat1, XMFLOAT4& Velocity2, const PhysicsMaterial& mat2, Manifold& m)
	{
		XMVECTOR rv = (XMVECTOR&)Velocity2 - (XMVECTOR&)Velocity1;

		float d = VectorDot((XMFLOAT3&)rv, m.Normal);

		float e = min(mat1.Restitution, mat2.Restitution);

		float j = max(-(1 + e) * d, 0);
		j /= (mat1.InvMass + mat2.InvMass);

		XMVECTOR Impulse = XMVectorSet(j * m.Normal.x, j * m.Normal.y, j * m.Normal.z, 0);

		Velocity1 = (XMFLOAT4&)((XMVECTOR&)Velocity1 - mat1.InvMass * Impulse);
		Velocity2 = (XMFLOAT4&)((XMVECTOR&)Velocity2 + mat2.InvMass * Impulse);
	}
}