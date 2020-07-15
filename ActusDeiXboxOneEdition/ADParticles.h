#pragma once
#include "pch.h"
#include "Types.h"
#include "Utils.h"

#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
using namespace DirectX;
using namespace std;

namespace
{
	float RandFloat(float min, float max)
	{
		float ratio = rand() / (float)RAND_MAX;
		return (max - min) * ratio + min;
	}
	const float GRAVITY = -10.0f;
	const int numParticles = 4096;
}

struct ParticleConstantBuffer
{
	XMFLOAT4X4 WorldMatrix;
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectionMatrix;
	XMFLOAT4 camPos;
	XMFLOAT4 Time;
	XMFLOAT4 Dimensions;
};

struct ParticlePositionConstantBuffer
{
	XMFLOAT4 positions[numParticles];
};

struct ParticleAttributes
{
	XMFLOAT4 position;
	XMFLOAT4 color;
};

class Particle
{
public:
	Particle();

	void Update(double time, XMFLOAT4 newVelocity, XMFLOAT4 newPosition = { 0, 0, 0, 1 });

	XMFLOAT4 GetPosition();
	ParticleAttributes* GetAttributes();
	float GetWidth();
	float GetHeight();
	float GetElaspedTime();
	float GetLifeSpan();
	void SetPosition(XMFLOAT4 position);
	void SetColor(XMFLOAT4 color);
	void SetGravityEffect(float gEffect);
	void SetWidth(float newWidth);
	void SetHeight(float newHeight);
	void SetLifeSpan(float _life);
	void Reset();

private:
	ParticleAttributes attributes;
	XMFLOAT4 velocity;
	float width;
	float height;
	float gravityEffect;
	float lifeSpan;
	float elaspedTime = 0.0f;
};

class ParticleRenderer
{
public:
	ComPtr<ID3D11InputLayout> inputLayout;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11ShaderResourceView> textureResourceView;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11GeometryShader> geometryShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	XMMATRIX worldMatrix;
	Particle* particleToRender;
	ComPtr<ID3D11Buffer> particleCBuff;
	ComPtr<ID3D11Buffer> particlePosCBuff;
	ParticleConstantBuffer particleConstants;
	ParticlePositionConstantBuffer particlePositions;

	ParticleRenderer();
	void CreateConstantBuffer(ComPtr<ID3D11Buffer>& ConstantBuffer, ID3D11Device* Device, int SizeOfConstantBuffer);
	void CreateVertexBuffer(ID3D11Device* Device);
	void CreateShadersAndInputLayout(ID3D11Device* Device, ADUtils::SHADER& shader);
	void CreateTexture(ID3D11Device* Device, const wchar_t* Filename);

	void RenderParticle(ID3D11DeviceContext* DeviceContext, int InstanceAmount, float u = 0, float v = 0);
};



class FountainEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
};

class SpreadEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
};

class CylinderEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, float radius, const wchar_t* textureName);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition, float newRadius);
	XMFLOAT4 GetPosition();
	float GetRadius();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float emitterRadius;
	vector<XMFLOAT4> cylinderPoints;
	int emitterSlices;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};

class BigCloudEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};

class SmallCloudEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};

class UpwardCloudEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
	bool GetActive();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};

class OutwardCloudEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};

class LongForwardCloudEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};

class WaveEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	float totalElaspedTime = 0.0f;
	bool isActive;
};

class AnimSpreadEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);

	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	float coordTime;
	float uCoord;
	float vCoord;
};

class HealthEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, float radius, const wchar_t* textureName);

	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);

	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition, float newRadius);
	XMFLOAT4 GetPosition();
	float GetRadius();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float emitterRadius;
	vector<XMFLOAT4> cylinderPoints;
	int emitterSlices;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};

class EssenceEmitter
{
public:
	void Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	Particle particle;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	bool isActive;
};

class BloodEmitter
{
public:
	void Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	Particle particle;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	bool isActive;
	float coordTime;
	float uCoord;
	float vCoord;
};

class BigElementalPuffEmitter
{
public:
	void Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName, XMFLOAT4 Color);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	Particle particle;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	bool isActive;
	float coordTime;
	float uCoord;
	float vCoord;
	XMFLOAT4 color;
};

class SmallElementalPuffEmitter
{
public:
	void Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(XMFLOAT4 newPosition, XMFLOAT4 newColor);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	Particle particle;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	bool isActive;
	float coordTime;
	float uCoord;
	float vCoord;
};

class FireballEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};

class IronSkinEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f);
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos);
	void RenderParticles(ID3D11DeviceContext* deviceContext);
	void Activate(float newLife, XMFLOAT4 newPosition);
	XMFLOAT4 GetPosition();
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};