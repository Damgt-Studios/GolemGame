#include "pch.h"
#include "Camera.h"

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{

	XMVECTOR position, targetPosition, up;

	position = XMLoadFloat3(&m_position);
	targetPosition = XMLoadFloat3(&m_targetPosition);
	up = XMLoadFloat3(&m_up);

	viewMatrix = XMMatrixLookAtLH(position, targetPosition, up);
	XMMATRIX InvView = XMMatrixInverse(nullptr, viewMatrix);
	transform = XMMatrixTranslation(InvView.r[3].m128_f32[0], InvView.r[3].m128_f32[1], InvView.r[3].m128_f32[2]);
	collider = OBB(transform, XMFLOAT3(20, 20, 20));
}


DirectX::XMFLOAT3& Camera::GetRight()
{
	return m_right;
}

DirectX::XMFLOAT3& Camera::GetLook()
{
	return m_look;
}

DirectX::XMFLOAT3& Camera::GetUp()
{
	return m_up;
}

DirectX::XMFLOAT3& Camera::GetPosition()
{
	return m_position;
}

XMFLOAT3& Camera::GetRotationEuler()
{
	XMMATRIX viewMatrix;
	GetViewMatrix(viewMatrix);


	viewMatrix = XMMatrixInverse(nullptr, viewMatrix);

	float Yaw; float Pitch; float Roll;
	if (viewMatrix.r[0].m128_f32[0] == 1.0f)
	{
		Yaw = atan2f(viewMatrix.r[0].m128_f32[2], viewMatrix.r[2].m128_f32[3]);
		Pitch = 0;
		Roll = 0;

	}
	else if (viewMatrix.r[0].m128_f32[0] == -1.0f)
	{
		Yaw = atan2f(viewMatrix.r[0].m128_f32[2], viewMatrix.r[2].m128_f32[3]);
		Pitch = 0;
		Roll = 0;
	}
	else
	{

		Yaw = atan2(-viewMatrix.r[2].m128_f32[0], viewMatrix.r[0].m128_f32[0]);
		Pitch = asin(viewMatrix.r[1].m128_f32[0]);
		Roll = atan2(-viewMatrix.r[1].m128_f32[2], viewMatrix.r[1].m128_f32[1]);
	}

	return XMFLOAT3{ Pitch, Yaw, Roll };
}

float Camera::GetFOV()
{
	return m_fov;
}

float Camera::GetFOVDegrees()
{
	return XMConvertToDegrees(m_fov);
}

float Camera::GetNear()
{
	return m_near;
}

float Camera::GetFar()
{
	return m_far;
}

//************************************
// Method:    SetFOV
// FullName:  Camera::SetFOV
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: float fov - in degrees (function converts to radians)
//************************************
void Camera::SetFOV(float fov)
{
	m_fov = XMConvertToRadians(fov);
}

void Camera::SetClippingPlanes(float _near, float _far)
{
	m_near = _near; m_far = _far;
}

Camera::Camera()
{
	collider = OBB(transform, XMFLOAT3(20, 20, 20));
	colliderPtr = &collider;


	physicsType = (int)OBJECT_PHYSICS_TYPE::TRIGGER;
	m_position = XMFLOAT3(0, 0, 0);
	m_targetPosition = XMFLOAT3(0, 0, 0);
	m_up = XMFLOAT3(0, 1, 0);
	m_right = XMFLOAT3(0, 0, 0);
	m_worldUp = XMFLOAT3(0, 1, 0);
	m_yaw = WMATH_PI;
	m_fov = WMATH_PI / 4; // default fov
	m_near = .01;
	m_far = 3000;
}


//************************************
//            FPS CAMERA
//************************************
FPSCamera::FPSCamera(XMFLOAT3 position /*= XMFLOAT3(0, 0, 0)*/, float yaw /*= PI*/, float pitch /*= 0*/)
{
	m_position = position;
	m_yaw = yaw;
	m_pitch = pitch;
}

void FPSCamera::SetPosition(XMFLOAT3 position)
{
	m_position = position;
	UpdateCameraVectors();
}

void FPSCamera::Rotate(float yaw, float pitch)
{
	
	// Convert the yaw and pitch to radians
	m_yaw += XMConvertToRadians(yaw);
	m_pitch += XMConvertToRadians(pitch);


	// Clamp the pitch between -180 and 180 exclusive
	if (m_pitch < (-WMATH_PI / 2) + .1) m_pitch = (-WMATH_PI / 2) + .1;
	else if (m_pitch > (WMATH_PI / 2) - .1) m_pitch = (WMATH_PI / 2) - .1;

	// Update the other shit
	UpdateCameraVectors();
}

void FPSCamera::Move(XMFLOAT3 position)
{
	XMVECTOR posOld, posNew, result;
	posOld = XMLoadFloat3(&m_position);
	posNew = XMLoadFloat3(&position);

	result = XMVectorAdd(posNew, posOld);
	XMStoreFloat3(&m_position, result);

	// Update the other shit
	UpdateCameraVectors();
}

void FPSCamera::UpdateCameraVectors()
{
	XMFLOAT3 look;

	look.x = cosf(m_pitch) * sinf(m_yaw);
	look.y = sinf(m_pitch);
	look.z = -(cosf(m_pitch) * cosf(m_yaw));

	XMVECTOR lookvector, worldupvector, rightvector, positionvector, upvector, targetpositionvector;

	lookvector = XMLoadFloat3(&look);
	worldupvector = XMLoadFloat3(&m_worldUp);
	rightvector = XMLoadFloat3(&m_right);
	positionvector = XMLoadFloat3(&m_position);

	// Normalize the look vector & store it
	XMVector3Normalize(lookvector);
	XMStoreFloat3(&m_look, lookvector);

	// Recompute the right and up vectors
	rightvector = XMVector3Normalize(XMVector3Cross(lookvector, worldupvector));
	upvector = XMVector3Normalize(XMVector3Cross(rightvector, lookvector));

	// Store right & up vector
	XMStoreFloat3(&m_right, rightvector);
	XMStoreFloat3(&m_up, upvector);

	targetpositionvector = positionvector + lookvector;

	// Store the new target position
	XMStoreFloat3(&m_targetPosition, targetpositionvector);
}

OrbitCamera::OrbitCamera() : mRadius(45.0f) {}

void OrbitCamera::Rotate(float &yaw, float& pitch)
{
	m_yaw = XMConvertToRadians(yaw);
	m_pitch = XMConvertToRadians(pitch);

	m_pitch = clamp(m_pitch, (-WMATH_PI / 2.0f) + 0.1f, (WMATH_PI / 2.0f) - 0.1f);

	UpdateCameraVectors();
}
void OrbitCamera::OnCollision()
{
	 radiusChange+= 10;
	 changeView = true;
	
}
void OrbitCamera::SetColliderPosition(XMFLOAT3 m)
{
}
void OrbitCamera::CheckCollision(ADResource::ADGameplay::GameObject* obj)
{

	Manifold m;

	if (obj->active)
	{
		if (obj->colliderPtr->isCollision(&collider, m))
		{
			//If collision and collision object is a trigger then go to OnTrigger Function
			if (obj->colliderPtr->trigger)
		{
				//OnTrigger(obj);
			}
			//If collision and collision object is a collider then go to OnCollision Function
			else
			{

				if (obj->physicsType == (int)OBJECT_PHYSICS_TYPE::COLLIDABLE || obj->physicsType == (int)OBJECT_PHYSICS_TYPE::STATIC)
				{
					collisionQueue.push(CollisionPacket(obj, this, m));
					OnCollision();
				}

				

				


			}
		}
		
	


	}
	

}
void OrbitCamera::SetLookAt(XMFLOAT3 target)
{
	m_targetPosition = target;
}

void OrbitCamera::SetRadius(float radius)
{

	mRadius = clamp(radius, 2.0f, 400.0f);
}


void OrbitCamera::SetLookAtAndRotate(XMFLOAT3 lookat, float yaw, float pitch, float delta_time)
{
	Rotate(yaw, pitch);
	m_targetPosition = lookat;
}


void OrbitCamera::SetRunningTarget(XMFLOAT3 rt)
{
	running_target = rt;
}

void OrbitCamera::Update(XMFLOAT3 lookat, float yaw, float pitch, float delta_time)
{

}

void OrbitCamera::UpdateCameraVectors()
{
	
	if (radiusChange > 300.0f)
	{
		radiusChange = 45.0f;
	}
	SetRadius(radiusChange);
	
	
	// Spherical to Cartesian coordinates
	m_position.x = m_targetPosition.x + mRadius * cosf(m_pitch) * sinf(m_yaw);
	m_position.y = m_targetPosition.y + mRadius * sinf(m_pitch);
	m_position.z = m_targetPosition.z + mRadius * cosf(m_pitch) * cosf(m_yaw);


	if (radiusChange > 45)
	{
		radiusChange -= 0.1f;
	}



}

float clamp(float value, float min, float max)
{
	float temp = value;

	assert(min <= max || max > min);

	if (temp > max) temp = max;
	if (temp < min) temp = min;

	assert(temp >= min && temp <= max);

	return temp;
}

float lerp(float a, float b, float f)
{
	return (a * (1.0 - f)) + (b * f);
}