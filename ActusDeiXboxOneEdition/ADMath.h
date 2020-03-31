#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

inline namespace ADMath
{
	inline float Lerp(float a, float b, float r) { return a + (b - a) * r; };
	inline XMFLOAT4 VectorToFloat4(const XMVECTOR& vector) { XMFLOAT4 temp; XMStoreFloat4(&temp, vector); return temp; };
	inline XMVECTOR Float3ToVector(const XMFLOAT3& floats) { return XMLoadFloat3(&floats); }
	inline XMVECTOR Float4ToVector(const XMFLOAT4& floats) { return XMLoadFloat4(&floats); };
	inline XMFLOAT4X4 MatrixToFloat4x4(const XMMATRIX& matrix) { XMFLOAT4X4 temp; XMStoreFloat4x4(&temp, matrix); return temp; };
	inline XMMATRIX Float4x4ToMatrix(const XMFLOAT4X4& floats) { return XMLoadFloat4x4(&floats); };
	inline XMMATRIX MatrixMultiply(const XMMATRIX& mat1, const XMMATRIX& mat2) { return XMMatrixMultiply(mat1, mat2); }
	inline XMFLOAT3 Float3Multiply(const XMFLOAT3& float1, float multiplier) { return XMFLOAT3(float1.x * multiplier, float1.y * multiplier, float1.z * multiplier); };
	inline float VectorDot(const XMFLOAT3& float1, const XMFLOAT3& float2) { return float1.x * float2.x + float1.y * float2.y + float1.z * float2.z; };
	inline float VectorDot(const XMFLOAT4& float1, const XMFLOAT4& float2) { return float1.x * float2.x + float1.y * float2.y + float1.z * float2.z; };
	inline float VectorDot(const XMVECTOR& vector, const XMFLOAT4& float2) { XMFLOAT4 float1 = VectorToFloat4(vector); return VectorDot(float1, float2); };
	inline float VectorDot(const XMFLOAT4& float1, const XMVECTOR& vector) { XMFLOAT4 float2 = VectorToFloat4(vector); return VectorDot(float1, float2); };
	inline float VectorDot(const XMVECTOR& vector1, const XMVECTOR& vector2) { XMFLOAT4 float1 = VectorToFloat4(vector1); XMFLOAT4 float2 = VectorToFloat4(vector2); return VectorDot(float1, float2); };
	inline XMVECTOR VectorLerp(const XMVECTOR& vector1, const XMVECTOR& vector2, float ratio) { return XMVectorLerp(vector1, vector2, ratio); };
	inline XMVECTOR VectorLerp(const XMFLOAT4& float1, const XMVECTOR& vector2, float ratio) { XMVECTOR vector1 = Float4ToVector(float1); return VectorLerp(vector1, vector2, ratio); };
	inline XMVECTOR VectorLerp(const XMVECTOR& vector1, const XMFLOAT4& float2, float ratio) { XMVECTOR vector2 = Float4ToVector(float2); return VectorLerp(vector1, vector2, ratio); };
	inline XMVECTOR VectorLerp(const XMFLOAT4& float1, const XMFLOAT4& float2, float ratio) { XMVECTOR vector1 = Float4ToVector(float1), vector2 = Float4ToVector(float2); VectorLerp(vector1, vector2, ratio); };
	inline XMMATRIX MatrixLerp(const XMMATRIX& mat1, const XMMATRIX& mat2, float ratio)
	{
		XMVECTOR quaternion1 = XMQuaternionNormalize(XMQuaternionRotationMatrix(mat1));
		XMVECTOR quaternion2 = XMQuaternionNormalize(XMQuaternionRotationMatrix(mat2));
		XMVECTOR quaternion3 = XMQuaternionSlerp(quaternion1, quaternion2, ratio);

		XMVECTOR position;
		position.m128_f32[0] = Lerp(mat1.r[3].m128_f32[0], mat2.r[3].m128_f32[0], ratio);
		position.m128_f32[1] = Lerp(mat1.r[3].m128_f32[1], mat2.r[3].m128_f32[1], ratio);
		position.m128_f32[2] = Lerp(mat1.r[3].m128_f32[2], mat2.r[3].m128_f32[2], ratio);
		position.m128_f32[3] = 1;

		XMMATRIX final = XMMatrixRotationQuaternion(quaternion3);
		return XMMATRIX(final.r[0], final.r[1], final.r[2], position);
	}

};

