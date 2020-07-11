#pragma once

#define WMATH_PI 3.1415926

#include <d3d11.h>
#include <DirectXMath.h>
#include "Types.h"

using namespace ADResource::ADGameplay;
using namespace ADPhysics;
using namespace DirectX;


class Camera : public GameObject
{
public:
	void GetViewMatrix(XMMATRIX& viewMatrix);
	OBB collider;
	bool changeView = false;
	float radiusChange = 45;

	virtual void SetPosition(XMFLOAT3& position) {};
	virtual void Rotate(float yaw, float pitch) {};
	virtual void Move(XMFLOAT3& position) {};
	XMFLOAT3& GetRight();
	XMFLOAT3& GetLook();
	XMFLOAT3& GetUp();
	XMFLOAT3& GetPosition();
	XMFLOAT3& GetRotationEuler();
	float GetFOV();
	float GetFOVDegrees();
	float GetNear();
	float GetFar();
	void SetFOV(float fov);
	void SetClippingPlanes(float _near, float _far);

protected:
	Camera();

protected:
	XMFLOAT3 m_position, m_targetPosition, m_up, m_look, m_right, m_worldUp;

	float m_yaw, m_pitch, m_fov;
	float m_near, m_far;
};

class FPSCamera : public Camera
{
public:
	FPSCamera(XMFLOAT3 position = XMFLOAT3(0, 0, 0), float yaw = WMATH_PI, float pitch = 0);

	virtual void SetPosition(XMFLOAT3 position);
	virtual void Rotate(float yaw, float pitch);
	virtual void Move(XMFLOAT3 position);

private:
	void UpdateCameraVectors();
};

class OrbitCamera : public Camera
{
public:
	OrbitCamera();
	virtual void Rotate(float& yaw, float& pitch);
	void OnCollision();
	float min_pitch = -90;
	void SetColliderPosition(XMFLOAT3 m );

	void CheckCollision(ADResource::ADGameplay::GameObject* obj);
	void SetLookAt(XMFLOAT3 target);
	void SetRadius(float radius);

	void SetLookAtAndRotate(XMFLOAT3 lookat, float yaw, float pitch, float delta_time);

	// Customs
	void SetRunningTarget(XMFLOAT3 rt);
	void Update(XMFLOAT3 lookat, float yaw, float pitch, float delta_time);
private:
	void UpdateCameraVectors();

	float mRadius;

	XMFLOAT3 running_target;
	float catchup_delay = .2;
	float rotation_speed = 20;
	float follow_speed = 50;
};

// hELPERS
float clamp(float value, float min, float max);
float lerp(float a, float b, float f);