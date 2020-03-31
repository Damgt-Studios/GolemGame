#pragma once
#include "DirectXMath.h"
#include "ADMath.h"

namespace ADPhysics 
{
    struct OBB
    {
        XMFLOAT3 Pos, AxisX, AxisY, AxisZ, Half_size;
    };

    static float Gravity = -9.8f;
    static float maxDownwardVelocity = -20.0f;

    static bool SerperatingAxisTest(const XMFLOAT3& Difference, const XMFLOAT3& Plane, const OBB& box1, const OBB& box2)
    {
        return (fabs(VectorDot(Difference, Plane)) >
            (fabs(VectorDot((Float3Multiply(box1.AxisX, box1.Half_size.x)), Plane)) +
                fabs(VectorDot((Float3Multiply(box1.AxisY, box1.Half_size.y)), Plane)) +
                fabs(VectorDot((Float3Multiply(box1.AxisZ, box1.Half_size.z)), Plane)) +
                fabs(VectorDot((Float3Multiply(box2.AxisX, box2.Half_size.x)), Plane)) +
                fabs(VectorDot((Float3Multiply(box2.AxisY, box2.Half_size.y)), Plane)) +
                fabs(VectorDot((Float3Multiply(box2.AxisZ, box2.Half_size.z)), Plane))));
    }

    // test for separating planes in all 15 axes
    static bool ObbToObbCollision(const OBB& box1, const OBB& box2)
    {
        static XMFLOAT3 Difference;
        Difference = XMFLOAT3(box2.Pos.x - box1.Pos.x, box2.Pos.y - box1.Pos.y, box2.Pos.z - box1.Pos.z);

        return !(
            ADPhysics::SerperatingAxisTest(Difference, box1.AxisX, box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, box1.AxisY, box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, box1.AxisZ, box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, box2.AxisX, box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, box2.AxisY, box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, box2.AxisZ, box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisX), Float3ToVector(box2.AxisX)), box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisX), Float3ToVector(box2.AxisY)), box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisX), Float3ToVector(box2.AxisZ)), box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisY), Float3ToVector(box2.AxisX)), box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisY), Float3ToVector(box2.AxisY)), box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisY), Float3ToVector(box2.AxisZ)), box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisZ), Float3ToVector(box2.AxisX)), box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisZ), Float3ToVector(box2.AxisY)), box1, box2) ||
            ADPhysics::SerperatingAxisTest(Difference, (XMFLOAT3&)XMVector3Cross(Float3ToVector(box1.AxisZ), Float3ToVector(box2.AxisZ)), box1, box2));
    }
}