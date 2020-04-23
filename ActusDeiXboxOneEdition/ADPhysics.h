#pragma once
//#include "DirectXMath.h"
#include "ADMath.h"

namespace ADPhysics
{
	struct AABB
	{
		XMFLOAT3 Pos, Min, Max, Extents, HalfSize;
		AABB()
		{
			Pos = XMFLOAT3(0, 0, 0);
			HalfSize = XMFLOAT3(0.5f, 0.5f, 0.5f);
			Max = XMFLOAT3(Pos.x + HalfSize.x, Pos.y + HalfSize.y, Pos.z + HalfSize.z);
			Min = XMFLOAT3(Pos.x - HalfSize.x, Pos.y - HalfSize.y, Pos.z - HalfSize.z);
			Extents = XMFLOAT3(Max.x - Pos.x, Max.y - Pos.y, Max.z - Pos.z);
		};
		AABB(XMFLOAT3 Position, XMFLOAT3 Size)
		{
			Pos = Position;
			HalfSize = Size;
			Max = XMFLOAT3(Pos.x + HalfSize.x, Pos.y + HalfSize.y, Pos.z + HalfSize.z);
			Min = XMFLOAT3(Pos.x - HalfSize.x, Pos.y - HalfSize.y, Pos.z - HalfSize.z);
			Extents = XMFLOAT3(Max.x - Pos.x, Max.y - Pos.y, Max.z - Pos.z);
		};
	};

	struct OBB
	{
		XMFLOAT3 Pos, AxisX, AxisY, AxisZ, HalfSize;
		OBB()
		{
			Pos = XMFLOAT3(0, 0, 0);
			AxisX = XMFLOAT3(1, 0, 0);
			AxisY = XMFLOAT3(0, 1, 0);
			AxisZ = XMFLOAT3(0, 0, 1);
			HalfSize = XMFLOAT3(0.5f, 0.5f, 0.5f);
		};
		OBB(XMMATRIX Transform, XMFLOAT3 Size)
		{
			Pos = (XMFLOAT3&)Transform.r[3];
			AxisX = (XMFLOAT3&)Transform.r[0];
			AxisY = (XMFLOAT3&)Transform.r[1];
			AxisZ = (XMFLOAT3&)Transform.r[2];
			HalfSize = Size;
		}
	};

	struct Sphere
	{
		XMFLOAT3 Pos; float Radius;
		Sphere()
		{
			Pos = XMFLOAT3(0, 0, 0);
			Radius = 0.5f;
		}

		Sphere(XMFLOAT3 Position, float Radius) : Pos(Position), Radius(Radius) { };
	};

	struct Plane
	{
		XMFLOAT3 Pos, Normal, AxisX, AxisY, AxisZ, HalfSize;
		Plane()
		{
			Pos = XMFLOAT3(0, 0, 0);
			Normal = XMFLOAT3(0, 1, 0);
			AxisX = XMFLOAT3(1, 0, 0);
			AxisY = Normal;
			AxisZ = XMFLOAT3(0, 0, 1);
			HalfSize = XMFLOAT3(0.5f, 0.5f, 0.5f);
		}

		Plane(XMMATRIX Transform, XMFLOAT3 Size)
		{
			Pos = (XMFLOAT3&)Transform.r[3];
			AxisX = (XMFLOAT3&)Transform.r[0];
			AxisY = (XMFLOAT3&)Transform.r[1];
			AxisZ = (XMFLOAT3&)Transform.r[2];
			Normal = AxisY;
			HalfSize = Size;
		}
	};

	struct Raycast
	{
		XMFLOAT3 Start, End;
		Raycast() : Start(XMFLOAT3(0, 0, 0)), End(XMFLOAT3(0, 0, 0)) {};
		Raycast(XMFLOAT3 Begin, XMFLOAT3 End) : Start(Begin), End(End) {};
	};

	struct AxisPoint
	{
		XMFLOAT3 Pos, AxisX, AxisY, AxisZ;
		AxisPoint()
		{
			Pos = XMFLOAT3(0, 0, 0);
			AxisX = XMFLOAT3(1, 0, 0);
			AxisY = XMFLOAT3(0, 1, 0);
			AxisZ = XMFLOAT3(0, 0, 1);
		}

		AxisPoint(XMFLOAT3 Position, XMFLOAT3 X, XMFLOAT3 Y, XMFLOAT3 Z) : Pos(Position), AxisX(X), AxisY(Y), AxisZ(Z) { };

		AxisPoint(XMMATRIX Transform)
		{
			Pos = (XMFLOAT3&)Transform.r[3];
			AxisX = (XMFLOAT3&)Transform.r[0];
			AxisY = (XMFLOAT3&)Transform.r[1];
			AxisZ = (XMFLOAT3&)Transform.r[2];
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

	static bool SerparatingAxisTest(const XMFLOAT3& Difference, const XMFLOAT3& Axis, const OBB& box, const AxisPoint& point)
	{
		return (fabs(VectorDot(Difference, Axis)) >
			(fabs(VectorDot((Float3Multiply(box.AxisX, box.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(box.AxisY, box.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(box.AxisZ, box.HalfSize.z)), Axis)) +
				fabs(VectorDot((Float3Multiply(point.AxisX, 0.1f)), Axis)) +
				fabs(VectorDot((Float3Multiply(point.AxisY, 0.1f)), Axis)) +
				fabs(VectorDot((Float3Multiply(point.AxisZ, 0.1f)), Axis))));
	}

	static bool SerparatingAxisTest(const XMFLOAT3& Difference, const XMFLOAT3& Axis, const Plane& plane, const AxisPoint& point)
	{
		return (fabs(VectorDot(Difference, Axis)) >
			(fabs(VectorDot((Float3Multiply(plane.AxisX, plane.HalfSize.x)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisY, plane.HalfSize.y)), Axis)) +
				fabs(VectorDot((Float3Multiply(plane.AxisZ, plane.HalfSize.z)), Axis)) +
				fabs(VectorDot(point.AxisX, Axis)) +
				fabs(VectorDot(point.AxisY, Axis)) +
				fabs(VectorDot(point.AxisZ, Axis))));
	}

	//------------------------------------------Distance Formula

	static float CalculateDistance(const XMFLOAT3& first, const XMFLOAT3& second)
	{
		return sqrt(pow(second.x - first.x, 2) + pow(second.y - first.y, 2) + pow(second.z - first.z, 2));
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

		XMVECTOR Vector = (XMVECTOR&)(box2.Pos)-(XMVECTOR&)(box1.Pos);
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

	static bool ObbToObbCollision(const OBB& box1, const OBB& box2, Manifold &m)
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

		return true; // Seperating axis not found
	}

	//------------------------------------------Raycast Collisions

	static bool RaycastToSphere(const Raycast& ray, const Sphere sphere)
	{
		XMVECTOR StartToEnd = (XMVECTOR&)ray.End - (XMVECTOR&)ray.Start;
		StartToEnd = XMVector3Normalize(StartToEnd);
		XMVECTOR StartToTest = (XMVECTOR&)sphere.Pos - (XMVECTOR&)ray.Start;
		float ClosestPointDist = VectorDot(StartToEnd, StartToTest);
		XMFLOAT3 ClosestPoint;
		if (ClosestPointDist < 0)
			ClosestPoint = ray.Start;
		else if (ClosestPointDist > ADPhysics::CalculateDistance(ray.Start, ray.End))
			ClosestPoint = ray.End;
		else
		{
			XMVECTOR temp = StartToEnd * ClosestPointDist;
			ClosestPoint = (XMFLOAT3&)((XMVECTOR&)ray.Start + temp);
		}
		float Distance = ADPhysics::CalculateDistance(ClosestPoint, sphere.Pos);
		return Distance < sphere.Radius;
	}

	static bool RaycastToAabb(const Raycast& ray, const AABB& box)
	{
		XMVECTOR StartToEnd = (XMVECTOR&)ray.End - (XMVECTOR&)ray.Start;
		float Radius = abs(VectorDot(box.Extents, (XMFLOAT3&)StartToEnd));
		Sphere temp = Sphere(box.Pos, Radius);
		return ADPhysics::RaycastToSphere(ray, temp);
	}

	static bool RaycastToObb(const Raycast& ray, const OBB& box)
	{
		XMVECTOR Z = Float3ToVector(ray.End) - Float3ToVector(ray.Start);
		Z = XMVector3Normalize(Z);

		XMVECTOR X = XMVector3Cross({ 0,1,0 }, Z);
		X = XMVector3Normalize(X);

		XMVECTOR Y = XMVector3Cross(Z, X);
		Y = XMVector3Normalize(Y);

		AxisPoint point = AxisPoint({ 0,0,0 }, (XMFLOAT3&)X, (XMFLOAT3&)Y, (XMFLOAT3&)Z);

		XMVECTOR StartToTest = (XMVECTOR&)box.Pos - (XMVECTOR&)ray.Start;
		float ClosestPointDist = VectorDot(Z, StartToTest);
		XMFLOAT3 ClosestPoint;
		if (ClosestPointDist < 0)
			ClosestPoint = ray.Start;
		else if (ClosestPointDist > CalculateDistance(ray.Start, ray.End))
			ClosestPoint = ray.End;
		else
		{
			XMVECTOR temp = Z * ClosestPointDist;
			ClosestPoint = (XMFLOAT3&)((XMVECTOR&)ray.Start + temp);
		}

		point.Pos = ClosestPoint;

		static XMFLOAT3 Difference;
		Difference = XMFLOAT3(point.Pos.x - box.Pos.x, point.Pos.y - box.Pos.y, point.Pos.z - box.Pos.z);

		return !(
			ADPhysics::SerparatingAxisTest(Difference, box.AxisX, box, point) ||
			ADPhysics::SerparatingAxisTest(Difference, box.AxisY, box, point) ||
			ADPhysics::SerparatingAxisTest(Difference, box.AxisZ, box, point) ||
			ADPhysics::SerparatingAxisTest(Difference, point.AxisX, box, point) ||
			ADPhysics::SerparatingAxisTest(Difference, point.AxisY, box, point) ||
			ADPhysics::SerparatingAxisTest(Difference, point.AxisZ, box, point));
	}

	static bool RaycastToPlane(const Raycast& ray, const Plane& plane)
	{
		if (VectorDot(ray.Start, plane.Normal) - VectorDot(plane.Pos, plane.Normal) < 0 &&
			VectorDot(ray.End, plane.Normal) - VectorDot(plane.Pos, plane.Normal) < 0)
			return false;

		if (VectorDot(ray.Start, plane.Normal) - VectorDot(plane.Pos, plane.Normal) > 0 &&
			VectorDot(ray.End, plane.Normal) - VectorDot(plane.Pos, plane.Normal) > 0)
			return false;

		XMVECTOR Z = Float3ToVector(ray.End) - Float3ToVector(ray.Start);
		Z = XMVector4Normalize(Z);

		XMVECTOR X = XMVector3Cross({ 0,1,0 }, Z);
		X = XMVector3Normalize(X);

		XMVECTOR Y = XMVector3Cross(Z, X);
		Y = XMVector3Normalize(Y);

		AxisPoint point = AxisPoint({ 0,0,0 }, (XMFLOAT3&)X, (XMFLOAT3&)Y, (XMFLOAT3&)Z);

		XMVECTOR StartToTest = (XMVECTOR&)plane.Pos - (XMVECTOR&)ray.Start;
		float ClosestPointDist = VectorDot(Z, StartToTest);
		XMFLOAT3 ClosestPoint;
		if (ClosestPointDist < 0)
			ClosestPoint = ray.Start;
		else if (ClosestPointDist > CalculateDistance(ray.Start, ray.End))
			ClosestPoint = ray.End;
		else
		{
			XMVECTOR temp = Z * ClosestPointDist;
			ClosestPoint = (XMFLOAT3&)((XMVECTOR&)ray.Start + temp);
		}

		point.Pos = ClosestPoint;

		static XMFLOAT3 Difference;
		Difference = XMFLOAT3(point.Pos.x - plane.Pos.x, point.Pos.y - plane.Pos.y, point.Pos.z - plane.Pos.z);

		return !(
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisX, plane, point) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisY, plane, point) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisZ, plane, point) ||
			ADPhysics::SerparatingAxisTest(Difference, point.AxisX, plane, point) ||
			ADPhysics::SerparatingAxisTest(Difference, point.AxisY, plane, point) ||
			ADPhysics::SerparatingAxisTest(Difference, point.AxisZ, plane, point));
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
		Difference = XMFLOAT3(plane.Pos.x - box.Pos.x, plane.Pos.y - box.Pos.y, plane.Pos.z - box.Pos.z);

		bool temp = !(
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(1, 0, 0), box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 1, 0), box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 0, 1), box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisX, box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisY, box, plane) ||
			ADPhysics::SerparatingAxisTest(Difference, plane.AxisZ, box, plane));

		if (temp == false)
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

		XMFLOAT3 tempF = (XMFLOAT3&)(((XMVECTOR&)tempX + (XMVECTOR&)tempY + (XMVECTOR&)tempZ) * (XMVECTOR&)obb.HalfSize);
		float Distance = ADPhysics::CalculateDistance((XMFLOAT3&)ClosestPoint, plane.Pos);
		float radius = (abs(plane.Normal.x) * abs(tempF.x)) + (abs(plane.Normal.y) * abs(tempF.y)) + (abs(plane.Normal.z) * abs(tempF.z));
		float overlap = radius - Distance;

		m.Normal = plane.Normal;
		m.PenetrationDepth = overlap;

		return true;
	}

	//------------------------------------------Find Closest Point from Origin To Target

	//static XMFLOAT3 ClosestPoint(const XMFLOAT3& point, const Sphere& sphere)
	//{
	//	XMVECTOR sphereToPoint = ((XMVECTOR&)point - (XMVECTOR&)sphere.Pos);
	//	sphereToPoint = XMVector3Normalize(sphereToPoint);
	//	sphereToPoint *= sphere.Radius;
	//	return (XMFLOAT3&)((XMVECTOR&)sphere.Pos + sphereToPoint);
	//}

	//static XMFLOAT3 ClosestPoint(const XMFLOAT3& point, const AABB& aabb)
	//{
	//	XMFLOAT3 result = point;
	//	XMFLOAT3 max = aabb.Max;
	//	XMFLOAT3 min = aabb.Min;

	//	result.x = result.x < min.x ? min.x : result.x;
	//	result.y = result.y < min.y ? min.y : result.y;
	//	result.z = result.z < min.z ? min.z : result.z;

	//	result.x = result.x > max.x ? max.x : result.x;
	//	result.y = result.y > max.x ? max.y : result.y;
	//	result.z = result.z > max.x ? max.z : result.z;

	//	return result;
	//}

	/*static XMFLOAT3 ClosestPointToBox(const XMFLOAT3& point, const OBB& obb)
	{
		XMFLOAT3 result = obb.Pos;
		XMFLOAT3 Direction = (XMFLOAT3&)((XMVECTOR&)point - (XMVECTOR&)result);

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
	}*/

	////------------------------------------------Miscellaneous Collision
	//static bool SphereToAabbCollision(const Sphere& sphere, const AABB& box)
	//{
	//	XMFLOAT3 Closest = ClosestPoint(sphere.Pos, box);
	//	XMFLOAT3 temp = (XMFLOAT3&)((XMVECTOR&)sphere.Pos - (XMVECTOR&)Closest);
	//	float distSq = VectorDot(temp, temp);
	//	return distSq <= sphere.Radius * sphere.Radius;
	//}

	//static bool SphereToObbCollision(const Sphere& sphere, const OBB& box)
	//{
	//	XMFLOAT3 Closest = ClosestPoint(sphere.Pos, box);
	//	XMFLOAT3 temp = (XMFLOAT3&)((XMVECTOR&)sphere.Pos - (XMVECTOR&)Closest);
	//	float distSq = VectorDot(temp, temp);
	//	float radiusSq = sphere.Radius * sphere.Radius;
	//	return distSq < radiusSq;
	//}

	//static bool AabbToObbCollision(const AABB& aabb, const OBB& obb)
	//{
	//	static XMFLOAT3 Difference;
	//	Difference = XMFLOAT3(obb.Pos.x - aabb.Pos.x, obb.Pos.y - aabb.Pos.y, obb.Pos.z - aabb.Pos.z);

	//	return !(
	//		ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(1, 0, 0), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 1, 0), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, XMFLOAT3(0, 0, 1), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, obb.AxisX, aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, obb.AxisY, aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, obb.AxisZ, aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(1, 0, 0, 1), Float3ToVector(obb.AxisX)), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(1, 0, 0, 1), Float3ToVector(obb.AxisY)), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(1, 0, 0, 1), Float3ToVector(obb.AxisZ)), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 1, 0, 1), Float3ToVector(obb.AxisX)), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 1, 0, 1), Float3ToVector(obb.AxisY)), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 1, 0, 1), Float3ToVector(obb.AxisZ)), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 0, 1, 1), Float3ToVector(obb.AxisX)), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 0, 1, 1), Float3ToVector(obb.AxisY)), aabb, obb) ||
	//		ADPhysics::SerparatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(XMVectorSet(0, 0, 1, 1), Float3ToVector(obb.AxisZ)), aabb, obb));
	//}

	////Collision Resolution

	//static XMFLOAT4 SphereToSphereResolution(const Sphere& firstSphere, const Sphere& secondSphere)
	//{
	//	XMFLOAT4 Axis = (XMFLOAT4&)XMVector3Normalize(((XMVECTOR&)firstSphere.Pos - (XMVECTOR&)secondSphere.Pos));

	//	XMFLOAT3 firstMax = (XMFLOAT3&)((XMVECTOR&)firstSphere.Pos + XMVectorSet(abs(Axis.x), abs(Axis.y), abs(Axis.z), 1) * firstSphere.Radius);
	//	XMFLOAT3 firstMin = (XMFLOAT3&)((XMVECTOR&)firstSphere.Pos - XMVectorSet(abs(Axis.x), abs(Axis.y), abs(Axis.z), 1) * firstSphere.Radius);
	//	XMFLOAT3 secondMax = (XMFLOAT3&)((XMVECTOR&)secondSphere.Pos + XMVectorSet(abs(Axis.x), abs(Axis.y), abs(Axis.z), 1) * secondSphere.Radius);
	//	XMFLOAT3 secondMin = (XMFLOAT3&)((XMVECTOR&)secondSphere.Pos - XMVectorSet(abs(Axis.x), abs(Axis.y), abs(Axis.z), 1) * secondSphere.Radius);

	//	//end::debug_renderer::add_line((end::float3&)firstSphere.Pos, (end::float3&)firstMax, { 1,0,0,0 });
	//	//end::debug_renderer::add_line((end::float3&)firstSphere.Pos, (end::float3&)firstMin, { 0,1,0,0 });
	//	//end::debug_renderer::add_line((end::float3&)secondSphere.Pos, (end::float3&)secondMax, { 0,0,1,0 });
	//	//end::debug_renderer::add_line((end::float3&)secondSphere.Pos, (end::float3&)secondMin, { 0,0,0,1 });

	//	XMFLOAT3 collisionMax, collisionMin;
	//	collisionMax.x = min(firstMax.x, secondMax.x);
	//	collisionMax.y = min(firstMax.y, secondMax.y);
	//	collisionMax.z = min(firstMax.z, secondMax.z);

	//	collisionMin.x = max(firstMin.x, secondMin.x);
	//	collisionMin.y = max(firstMin.y, secondMin.y);
	//	collisionMin.z = max(firstMin.z, secondMin.z);

	//	XMFLOAT4 Overlap = VectorToFloat4((XMVECTOR&)collisionMax - (XMVECTOR&)collisionMin);


	//	XMVECTOR Force = (XMVECTOR&)Overlap * ((XMVECTOR&)Axis) * 0.5f;
	//	//Force = XMVectorSet(Force.m128_f32[0] * Epsilon, Force.m128_f32[1] * Epsilon, Force.m128_f32[2] * Epsilon, 1);

	//	return (XMFLOAT4&)Force;

	//	end::debug_renderer::add_line((end::float3&)collisionMax, (end::float3&)collisionMin, { 0,0,0,1 });

	//	return XMFLOAT4(0, 0, 0, 0);
	//}

	//static XMFLOAT4 AabbToAabbResolution(const AABB& firstBox, const AABB& secondBox)
	//{
	//	XMFLOAT3 collisionMax, collisionMin;
	//	collisionMax.x = min(firstBox.Max.x, secondBox.Max.x);
	//	collisionMax.y = min(firstBox.Max.y, secondBox.Max.y);
	//	collisionMax.z = min(firstBox.Max.z, secondBox.Max.z);

	//	collisionMin.x = max(firstBox.Min.x, secondBox.Min.x);
	//	collisionMin.y = max(firstBox.Min.y, secondBox.Min.y);
	//	collisionMin.z = max(firstBox.Min.z, secondBox.Min.z);

	//	XMFLOAT4 Overlap = VectorToFloat4((XMVECTOR&)collisionMax - (XMVECTOR&)collisionMin);
	//	XMFLOAT3 ContactNormal = XMFLOAT3();
	//	float PenetrationDepth = 0;

	//	if (Overlap.x <= Overlap.y && Overlap.x <= Overlap.z)
	//	{
	//		ContactNormal = XMFLOAT3(1, 0, 0);
	//		PenetrationDepth = Overlap.x;
	//	}
	//	else if (Overlap.y <= Overlap.x && Overlap.y <= Overlap.z)
	//	{
	//		ContactNormal = XMFLOAT3(0, 1, 0);
	//		PenetrationDepth = Overlap.y;
	//	}
	//	else if (Overlap.z <= Overlap.x && Overlap.z <= Overlap.y)
	//	{
	//		ContactNormal = XMFLOAT3(0, 0, 1);
	//		PenetrationDepth = Overlap.z;
	//	}

	//	XMVECTOR temp = (XMVECTOR&)(firstBox.Pos) - (XMVECTOR&)(secondBox.Pos);
	//	XMFLOAT4 tempF = VectorToFloat4(temp);

	//	float dot = tempF.x * ContactNormal.x + tempF.y * ContactNormal.y + tempF.z * ContactNormal.z;

	//	if (dot < 0)
	//		ContactNormal = XMFLOAT3(-ContactNormal.x, -ContactNormal.y, -ContactNormal.z);

	//	XMVECTOR Force = (((XMVECTOR&)Overlap * ((XMVECTOR&)(ContactNormal) * -1)) * 0.5f);

	//	//This is to find the point of collision and isn't needed for right now
	//	XMVECTOR cp = ((XMVECTOR&)collisionMax + (XMVECTOR&)collisionMin) * 0.5f;
	//	cp -= (XMVECTOR&)ContactNormal * PenetrationDepth * 0.5f;

	//	//Contact Point can be found and used to call Event System at this position
	//	XMFLOAT4 ContactPoint = VectorToFloat4(cp);

	//	//end::debug_renderer::add_line((end::float3&)ContactPoint, (end::float3&)ContactPoint + (end::float3&)ContactNormal, { 1,0,0,1 });

	//	return (XMFLOAT4&)Force;
	//}

	//static XMFLOAT4 ObbToObbResolution(const OBB& firstBox, const OBB& secondBox)
	//{
	//	return XMFLOAT4(0, 0, 0, 0);
	//}

	//static XMFLOAT4 SphereToPlaneResolution(const Sphere& sphere, const Plane& plane)
	//{
	//	XMVECTOR NormalPosition = XMVectorSet(plane.Pos.x + plane.Normal.x, plane.Pos.y + plane.Normal.y, plane.Pos.z + plane.Normal.z, 1);
	//	XMVECTOR L = XMVector3Normalize(NormalPosition - (XMVECTOR&)plane.Pos);
	//	XMVECTOR V = (XMVECTOR&)sphere.Pos - (XMVECTOR&)plane.Pos;

	//	float Dot = VectorDot(L, V);
	//	XMFLOAT4 ClosestPoint = (XMFLOAT4&)((XMVECTOR&)plane.Pos + (L * Dot));
	//	float Distance = CalculateDistance((XMFLOAT3&)ClosestPoint, plane.Pos);
	//	float overlap = sphere.Radius - Distance;
	//	return XMFLOAT4(plane.Normal.x * overlap, plane.Normal.y * overlap, plane.Normal.z * overlap, 1);
	//}

	//static XMFLOAT4 AabbToPlaneResolution(const AABB& aabb, const Plane& plane)
	//{
	//	//Draw Line from Plane position to planes normal
	//	XMVECTOR NormalPosition = XMVectorSet(plane.Pos.x + plane.Normal.x, plane.Pos.y + plane.Normal.y, plane.Pos.z + plane.Normal.z, 1);
	//	XMVECTOR L = XMVector3Normalize(NormalPosition - (XMVECTOR&)plane.Pos);
	//	//Get Closest Point from AABB to line
	//	XMVECTOR V = (XMVECTOR&)aabb.Pos - (XMVECTOR&)plane.Pos;
	//	float Dot = VectorDot(L, V);
	//	XMFLOAT4 ClosestPoint = (XMFLOAT4&)((XMVECTOR&)plane.Pos + (L * Dot));
	//	float Distance = ADPhysics::CalculateDistance((XMFLOAT3&)ClosestPoint, plane.Pos);
	//	float radius = (abs(plane.Normal.x) * aabb.Extents.x) + (abs(plane.Normal.y) * aabb.Extents.y) + (abs(plane.Normal.z) * aabb.Extents.z);
	//	float overlap = radius - Distance;
	//	return XMFLOAT4(plane.Normal.x * overlap, plane.Normal.y * overlap, plane.Normal.z * overlap, 1);
	//}

	//static XMFLOAT4 ObbToPlaneResolution(const OBB& obb, const Plane& plane)
	//{
	//	XMVECTOR NormalPosition = XMVectorSet(plane.Pos.x + plane.Normal.x, plane.Pos.y + plane.Normal.y, plane.Pos.z + plane.Normal.z, 1);
	//	XMVECTOR L = XMVector3Normalize(NormalPosition - (XMVECTOR&)plane.Pos);
	//	XMVECTOR V = (XMVECTOR&)obb.Pos - (XMVECTOR&)plane.Pos;
	//	float Dot = VectorDot(L, V);
	//	XMFLOAT4 ClosestPoint = (XMFLOAT4&)((XMVECTOR&)plane.Pos + (L * Dot));
	//	XMFLOAT3 tempX = obb.AxisX;
	//	XMFLOAT3 tempY = obb.AxisY;
	//	XMFLOAT3 tempZ = obb.AxisZ;
	//	float testDot;
	//	testDot = VectorDot(plane.Normal, tempX);
	//	if (testDot < 0)
	//		tempX = XMFLOAT3(-tempX.x, -tempX.y, -tempX.z);
	//	testDot = VectorDot(plane.Normal, tempY);
	//	if (testDot < 0)
	//		tempY = XMFLOAT3(-tempY.x, -tempY.y, -tempY.z);
	//	testDot = VectorDot(plane.Normal, tempZ);
	//	if (testDot < 0)
	//		tempZ = XMFLOAT3(-tempZ.x, -tempZ.y, -tempZ.z);

	//	XMFLOAT3 temp = (XMFLOAT3&)(((XMVECTOR&)tempX + (XMVECTOR&)tempY + (XMVECTOR&)tempZ) * (XMVECTOR&)obb.HalfSize);
	//	float Distance = ADPhysics::CalculateDistance((XMFLOAT3&)ClosestPoint, plane.Pos);
	//	float radius = (abs(plane.Normal.x) * abs(temp.x)) + (abs(plane.Normal.y) * abs(temp.y)) + (abs(plane.Normal.z) * abs(temp.z));
	//	float overlap = radius - Distance;
	//	return XMFLOAT4(plane.Normal.x * overlap, plane.Normal.y * overlap, plane.Normal.z * overlap, 0);
	//}

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