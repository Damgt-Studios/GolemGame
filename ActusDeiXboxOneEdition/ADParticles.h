#pragma once
#include "pch.h"
#include "Types.h"
#include "Utils.h"
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
	Particle()
	{
		attributes.position = { 0, 0, 0, 1 };
		attributes.color = { 1, 0, 0, 1 };
		velocity = { 0, 0, 0, 1 };
		gravityEffect = 0;
		lifeSpan = RandFloat(0, 3);
		width = 0.5f;
		height = 0.5f;
	}
	void Update(double time, XMFLOAT4 newVelocity, XMFLOAT4 newPosition = { 0, 0, 0, 1 }, float maxLife = 3.0f)
	{
		if (lifeSpan == -1)
		{
			velocity.y += GRAVITY * gravityEffect * (float)time;
			attributes.position.x += newVelocity.x * time;
			attributes.position.y += newVelocity.y * time;
			attributes.position.z += newVelocity.z * time;
		}
		else
		{
			elaspedTime += time;
			if (elaspedTime > lifeSpan && lifeSpan != -1.0f)
			{
				attributes.position = newPosition;
				velocity = { 0, 0, 0, 1 };
				elaspedTime = 0.0f;
				lifeSpan = RandFloat(0, maxLife);
				velocity = newVelocity;
			}
			else
			{
				velocity.y += GRAVITY * gravityEffect * (float)time;
				attributes.position.x += velocity.x * time;
				attributes.position.y += velocity.y * time;
				attributes.position.z += velocity.z * time;
			}
		}
	}
	XMFLOAT4 GetPosition()
	{
		return attributes.position;
	}
	ParticleAttributes* GetAttributes()
	{
		return &attributes;
	}
	float GetWidth()
	{
		return width;
	}
	float GetHeight()
	{
		return height;
	}
	float GetElaspedTime()
	{
		return elaspedTime;
	}
	float GetLifeSpan()
	{
		return lifeSpan;
	}
	void SetPosition(XMFLOAT4 position)
	{
		attributes.position = position;
	}
	void SetColor(XMFLOAT4 color)
	{
		attributes.color = color;
	}
	void SetGravityEffect(float gEffect)
	{
		gravityEffect = gEffect;
	}
	void SetWidth(float newWidth)
	{
		width = newWidth;
	}
	void SetHeight(float newHeight)
	{
		height = newHeight;
	}
	void SetLifeSpan(float _life)
	{
		lifeSpan = _life;
	}
	void Reset()
	{
		attributes.position = { 0, 0, 0, 1 };
		attributes.color = { 1, 0, 0, 1 };
		velocity = { 0, 0, 0, 0 };
		gravityEffect = 0;
		lifeSpan = RandFloat(0, 3);
		width = 0.5f;
		height = 0.5f;
	}
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

	ParticleRenderer()
	{
		inputLayout = nullptr;
		vertexBuffer = nullptr;
		textureResourceView = nullptr;
		vertexShader = nullptr;
		geometryShader = nullptr;
		pixelShader = nullptr;
		worldMatrix = XMMatrixIdentity();
		particleToRender = nullptr;
		particleCBuff = nullptr;
		particlePosCBuff = nullptr;
		particleConstants = {};
		particlePositions = {};
	}
	void CreateConstantBuffer(ComPtr<ID3D11Buffer>& ConstantBuffer, ID3D11Device* Device, int SizeOfConstantBuffer)
	{
		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(bDesc));

		bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bDesc.ByteWidth = SizeOfConstantBuffer;
		bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;
		bDesc.Usage = D3D11_USAGE_DYNAMIC;

		HRESULT hr = Device->CreateBuffer(&bDesc, nullptr, &ConstantBuffer);
	}
	void CreateVertexBuffer(ID3D11Device* Device)
	{
		D3D11_BUFFER_DESC bDesc;
		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&bDesc, sizeof(bDesc));
		ZeroMemory(&subData, sizeof(subData));

		bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bDesc.ByteWidth = sizeof(ParticleAttributes);
		bDesc.CPUAccessFlags = 0;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;
		bDesc.Usage = D3D11_USAGE_DEFAULT;

		subData.pSysMem = particleToRender->GetAttributes();
		subData.SysMemPitch = 0;
		subData.SysMemSlicePitch = 0;

		HRESULT hr = Device->CreateBuffer(&bDesc, &subData, &vertexBuffer);
	}
	void CreateShadersAndInputLayout(ID3D11Device* Device, ADUtils::SHADER& shader)
	{
		ComPtr<ID3D10Blob> vertexblob;
		ComPtr<ID3D10Blob> pixelblob;
		ComPtr<ID3D10Blob> geometryblob;

		Platform::String^ appInstallFolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
		std::string READ_PATH = std::string(appInstallFolder->Begin(), appInstallFolder->End()).append("\\");

		std::string vname(shader.vshader);
		std::string pname(shader.pshader);
		std::string gname(shader.gshader);

		std::string v = std::string(READ_PATH.begin(), READ_PATH.end()).append(vname);
		std::string p = std::string(READ_PATH.begin(), READ_PATH.end()).append(pname);
		std::string g = std::string(READ_PATH.begin(), READ_PATH.end()).append(gname);

		//The Whittington Bruh aka The Wruh
		std::string bruh = std::string(READ_PATH.begin(), READ_PATH.end());

		std::wstring vshadername(v.begin(), v.end());
		std::wstring pshadername(p.begin(), p.end());
		std::wstring gshadername(g.begin(), g.end());

		HRESULT result;

		result = D3DCompileFromFile(vshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_VS, D3DCOMPILE_DEBUG, 0, &vertexblob, nullptr);
		result = D3DCompileFromFile(pshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_PS, D3DCOMPILE_DEBUG, 0, &pixelblob, nullptr);
		result = D3DCompileFromFile(gshadername.c_str(), NULL, NULL, ADUtils::SHADER_ENTRY_POINT, ADUtils::SHADER_MODEL_GS, D3DCOMPILE_DEBUG, 0, &geometryblob, nullptr);
		assert(!FAILED(result));

		result = Device->CreateVertexShader(vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), nullptr, &vertexShader);
		result = Device->CreatePixelShader(pixelblob->GetBufferPointer(), pixelblob->GetBufferSize(), nullptr, &pixelShader);
		result = Device->CreateGeometryShader(geometryblob->GetBufferPointer(), geometryblob->GetBufferSize(), nullptr, &geometryShader);

		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		result = Device->CreateInputLayout(inputDesc, 2, vertexblob->GetBufferPointer(), vertexblob->GetBufferSize(), &inputLayout);
	}
	void CreateTexture(ID3D11Device* Device, const wchar_t* Filename)
	{
		HRESULT hr = CreateDDSTextureFromFile(Device, Filename, nullptr, &textureResourceView);
	}
	void RenderParticle(ID3D11DeviceContext* DeviceContext, int InstanceAmount, float u = 0, float v = 0)
	{
		ID3D11ShaderResourceView* ShaderResourceViews[] = { textureResourceView.Get() };
		DeviceContext->PSSetShaderResources(0, 1, ShaderResourceViews);

		DeviceContext->IASetInputLayout(inputLayout.Get());
		UINT Strides[] = { sizeof(ParticleAttributes) };
		UINT Offsets[] = { 0 };
		ID3D11Buffer* VertexBuffers[] = { vertexBuffer.Get() };
		DeviceContext->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		DeviceContext->VSSetShader(vertexShader.Get(), 0, 0);
		DeviceContext->GSSetShader(geometryShader.Get(), 0, 0);
		DeviceContext->PSSetShader(pixelShader.Get(), 0, 0);

		XMStoreFloat4x4(&particleConstants.WorldMatrix, worldMatrix);
		particleConstants.Dimensions = XMFLOAT4(particleToRender->GetWidth(), particleToRender->GetHeight(), u, v);

		D3D11_MAPPED_SUBRESOURCE gpuBuffer;
		HRESULT hr = DeviceContext->Map(particleCBuff.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
		*((ParticleConstantBuffer*)(gpuBuffer.pData)) = particleConstants;
		DeviceContext->Unmap(particleCBuff.Get(), 0);

		D3D11_MAPPED_SUBRESOURCE gpuBuffer2;
		hr = DeviceContext->Map(particlePosCBuff.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer2);
		*((ParticlePositionConstantBuffer*)(gpuBuffer2.pData)) = particlePositions;
		DeviceContext->Unmap(particlePosCBuff.Get(), 0);

		ID3D11Buffer* constantBuffers[] = { particleCBuff.Get() };
		DeviceContext->VSSetConstantBuffers(0, 1, constantBuffers);
		DeviceContext->GSSetConstantBuffers(0, 1, constantBuffers);
		DeviceContext->PSSetConstantBuffers(0, 1, constantBuffers);

		ID3D11Buffer* constantBuffers2[] = { particlePosCBuff.Get() };
		DeviceContext->VSSetConstantBuffers(1, 1, constantBuffers2);

		DeviceContext->UpdateSubresource(vertexBuffer.Get(), 0, NULL, particleToRender->GetAttributes(), 0, 0);
		DeviceContext->DrawInstanced(1, InstanceAmount, 0, 0);

		DeviceContext->GSSetShader(nullptr, 0, 0);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
};



class FountainEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(-0.25f);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		renderer.particleConstants.ViewMatrix = view;
		renderer.particleConstants.camPos = camPos;
		renderer.particleConstants.ProjectionMatrix = projection;
		renderer.particleConstants.Time = { time, 0,0,0 };
		elaspedTime += time;
		if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
			for (int i = 0; i < size; ++i)
			{
				XMFLOAT4 velocity;
				velocity.x = RandFloat(-1, 1);
				velocity.y = RandFloat(-1, 1);
				velocity.z = RandFloat(-1, 1);
				particles[i].Update(time, velocity);
				renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
			}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetGravityEffect(-1);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		renderer.particleConstants.ViewMatrix = view;
		renderer.particleConstants.camPos = camPos;
		renderer.particleConstants.ProjectionMatrix = projection;
		renderer.particleConstants.Time = { time, 0,0,0 };
		elaspedTime += time;
		if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
			for (int i = 0; i < size; ++i)
			{
				XMFLOAT4 velocity;
				velocity.x = RandFloat(-2, 2);
				velocity.y = RandFloat(-2, 2);
				velocity.z = RandFloat(-2, 2);
				particles[i].Update(time, velocity);
				renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
			}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetGravityEffect(0);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, float radius, const wchar_t* textureName)
	{
		isActive = false;
		lifeSpan = 0.0f;
		emitterRadius = radius;
		emitterSlices = radius * 16;
		emitterPos = Pos;
		size = amountOfParticles;
		float theta = 2.0f * 3.14159f / (float)emitterSlices;
		cylinderPoints.resize(emitterSlices);
		particles.resize(size);
		for (int i = 0; i < emitterSlices; ++i)
		{
			float x = radius * cos(i * theta);
			float z = radius * sin(i * theta);
			cylinderPoints[i] = XMFLOAT4(x, emitterPos.y, z, 1.0f);
		}
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetPosition(cylinderPoints[rand() % emitterSlices]);
			particle.SetHeight(radius);
			particle.SetWidth(radius);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = 0;
					velocity.y = 50;
					velocity.z = 0;
					particles[i].Update(time, velocity, cylinderPoints[rand() % emitterSlices]);
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition, float newRadius)
	{
		isActive = true;
		emitterPos = newPosition;
		emitterRadius = newRadius;
		float theta = 2.0f * 3.14159f / (float)emitterSlices;
		for (int i = 0; i < emitterSlices; ++i)
		{
			float x = emitterRadius * cos(i * theta);
			float z = emitterRadius * sin(i * theta);
			cylinderPoints[i] = XMFLOAT4(x, emitterPos.y, z, 1.0f);
		}
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetHeight(emitterRadius);
			particles[i].SetWidth(emitterRadius);
			particles[i].SetGravityEffect(0);
			particles[i].SetLifeSpan(newLife);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
	float GetRadius()
	{
		return emitterRadius;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		isActive = false;
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetLifeSpan(0);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = RandFloat(-20, 20);
					velocity.y = RandFloat(-20, 20);
					velocity.z = RandFloat(-20, 20);
					particles[i].Update(time, velocity);
					if (particles[i].GetElaspedTime() <= 0.0f)
					{
						particles[i].SetLifeSpan(3);
					}
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		isActive = true;
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetLifeSpan(0);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);

	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		isActive = false;
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetLifeSpan(0);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = RandFloat(-5, 5);
					velocity.y = RandFloat(-5, 5);
					velocity.z = RandFloat(-5, 5);
					particles[i].Update(time, velocity);
					if (particles[i].GetElaspedTime() <= 0.0f)
					{
						particles[i].SetLifeSpan(3);
					}
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		isActive = true;
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetLifeSpan(0);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		isActive = false;
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetLifeSpan(0);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = RandFloat(-20, 20);
					velocity.y = RandFloat(0, 50);
					velocity.z = RandFloat(-20, 20);
					particles[i].Update(time, velocity);
					if (particles[i].GetElaspedTime() <= 0.0f)
					{
						particles[i].SetLifeSpan(3);
					}
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		isActive = true;
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetLifeSpan(0);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
	bool GetActive()
	{
		return isActive;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		isActive = false;
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetLifeSpan(0);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = RandFloat(-20, 20);
					velocity.y = RandFloat(0, 20);
					velocity.z = RandFloat(-20, 20);
					particles[i].Update(time, velocity);
					if (particles[i].GetElaspedTime() <= 0.0f)
					{
						particles[i].SetLifeSpan(3);
					}
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		isActive = true;
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetLifeSpan(0);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		isActive = false;
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetLifeSpan(0);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
		renderer.worldMatrix = XMMatrixMultiply(XMMatrixRotationX(1.0472f), renderer.worldMatrix);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = RandFloat(-5, 5);
					velocity.y = RandFloat(0, 30);
					velocity.z = RandFloat(-5, 5);
					particles[i].Update(time, velocity);
					if (particles[i].GetElaspedTime() <= 0.0f)
					{
						particles[i].SetLifeSpan(1);
					}
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		isActive = true;
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetLifeSpan(0);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
		renderer.worldMatrix = XMMatrixMultiply(XMMatrixRotationX(1.0472f), renderer.worldMatrix);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		isActive = false;
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			totalElaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = RandFloat(-50, 50);
					velocity.y = sinf(totalElaspedTime * 10) * 20;
					velocity.z = RandFloat(-50, 50);
					particles[i].Update(time, velocity);
					if (particles[i].GetElaspedTime() <= 0.0f)
					{
						particles[i].SetLifeSpan(1);
					}
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		isActive = true;
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		totalElaspedTime = 0.0f;
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		uCoord = 0.0f;
		vCoord = 0.0f;
		coordTime = 0.0f;
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetHeight(10);
			particle.SetWidth(10);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particleanim_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		renderer.particleConstants.ViewMatrix = view;
		renderer.particleConstants.camPos = camPos;
		renderer.particleConstants.ProjectionMatrix = projection;
		renderer.particleConstants.Time = { time, 0,0,0 };
		elaspedTime += time;
		if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
			for (int i = 0; i < size; ++i)
			{
				XMFLOAT4 velocity;
				velocity.x = RandFloat(-2, 2);
				velocity.y = RandFloat(-2, 2);
				velocity.z = RandFloat(-2, 2);
				particles[i].Update(time, velocity);
				renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
			}
		if (coordTime > 0.04f)
		{
			uCoord += 1.0f;
			if (uCoord >= 4)
			{
				uCoord = 0;
				vCoord += 1.0f;
				if (vCoord >= 4)
				{
					uCoord = 0;
					vCoord = 0;
				}
			}
			coordTime = 0.0f;
		}
		coordTime += time;
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
			renderer.RenderParticle(deviceContext, size, uCoord, vCoord);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetGravityEffect(0);
			particles[i].SetHeight(10);
			particles[i].SetWidth(10);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, float radius, const wchar_t* textureName)
	{
		isActive = false;
		lifeSpan = 0.0f;
		emitterRadius = radius;
		emitterSlices = radius * 16;
		emitterPos = Pos;
		size = amountOfParticles;
		float theta = 2.0f * 3.14159f / (float)emitterSlices;
		cylinderPoints.resize(emitterSlices);
		particles.resize(size);
		for (int i = 0; i < emitterSlices; ++i)
		{
			float x = radius * cos(i * theta);
			float z = radius * sin(i * theta);
			cylinderPoints[i] = XMFLOAT4(emitterPos.x + x, emitterPos.y, emitterPos.z + z, 1.0f);
		}
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetPosition(cylinderPoints[rand() % emitterSlices]);
			particle.SetHeight(1);
			particle.SetWidth(1);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = 0;
					velocity.y = 50;
					velocity.z = 0;
					particles[i].Update(time, velocity, cylinderPoints[rand() % emitterSlices]);
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition, float newRadius)
	{
		isActive = true;
		emitterPos = newPosition;
		emitterRadius = newRadius;
		float theta = 2.0f * 3.14159f / (float)emitterSlices;
		for (int i = 0; i < emitterSlices; ++i)
		{
			float x = emitterRadius * cos(i * theta);
			float z = emitterRadius * sin(i * theta);
			cylinderPoints[i] = XMFLOAT4(x, emitterPos.y, z, 1.0f);
		}
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetHeight(1);
			particles[i].SetWidth(1);
			particles[i].SetGravityEffect(0);
			particles[i].SetLifeSpan(newLife);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
	float GetRadius()
	{
		return emitterRadius;
	}
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
	void Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName)
	{
		isActive = false;
		emitterPos = Pos;
		Particle tempParticle;
		tempParticle.SetGravityEffect(0);
		tempParticle.SetHeight(1);
		tempParticle.SetWidth(1);
		particle = tempParticle;
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = &particle;
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			XMFLOAT4 velocity;
			velocity.x = 0;
			velocity.y = sinf(elaspedTime * 3) * 5;
			velocity.z = 0;
			particle.Update(time, velocity);
			renderer.particlePositions.positions[0] = particle.GetPosition();
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if (isActive)
			renderer.RenderParticle(deviceContext, 1);
	}
	void Activate(XMFLOAT4 newPosition)
	{
		isActive = true;
		particle.Reset();
		particle.SetHeight(1);
		particle.SetWidth(1);
		particle.SetLifeSpan(-1);
		renderer.particlePositions.positions[0] = particle.GetPosition();
		elaspedTime = 0.0f;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName)
	{
		isActive = false;
		uCoord = 0.0f;
		vCoord = 0.0f;
		coordTime = 0.0f;
		emitterPos = Pos;
		Particle tempParticle;
		tempParticle.SetGravityEffect(0);
		tempParticle.SetHeight(10);
		tempParticle.SetWidth(10);
		particle = tempParticle;
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = &particle;
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particleanim_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			XMFLOAT4 velocity;
			velocity.x = 0;
			velocity.y = RandFloat(-2, 2);
			velocity.z = 0;
			particle.Update(time, velocity);
			renderer.particlePositions.positions[0] = particle.GetPosition();
			if (coordTime > 0.04f)
			{
				uCoord += 1.0f;
				if (uCoord >= 4)
				{
					uCoord = 0;
					vCoord += 1.0f;
					if (vCoord >= 4)
					{
						isActive = false;
						uCoord = 0;
						vCoord = 0;
					}
				}
				coordTime = 0.0f;
			}
			coordTime += time;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if (isActive)
			renderer.RenderParticle(deviceContext, 1, uCoord, vCoord);
	}
	void Activate(XMFLOAT4 newPosition)
	{
		isActive = true;
		particle.Reset();
		particle.SetGravityEffect(0);
		particle.SetHeight(10);
		particle.SetWidth(10);
		particle.SetLifeSpan(-1.0f);
		uCoord = vCoord = 0;
		renderer.particlePositions.positions[0] = particle.GetPosition();
		elaspedTime = 0.0f;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
	bool GetActive()
	{
		return isActive;
	}
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
	void Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName, XMFLOAT4 Color)
	{
		isActive = false;
		color = Color;
		uCoord = 0.0f;
		vCoord = 0.0f;
		coordTime = 0.0f;
		emitterPos = Pos;
		Particle tempParticle;
		tempParticle.SetGravityEffect(0);
		tempParticle.SetHeight(20);
		tempParticle.SetWidth(20);
		particle = tempParticle;
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = &particle;
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particlecolor_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particleanim_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			XMFLOAT4 velocity;
			velocity.x = 0;
			velocity.y = RandFloat(-2, 2);
			velocity.z = 0;
			particle.Update(time, velocity);
			renderer.particlePositions.positions[0] = particle.GetPosition();
			if (coordTime > 0.04f)
			{
				uCoord += 1.0f;
				if (uCoord >= 4)
				{
					uCoord = 0;
					vCoord += 1.0f;
					if (vCoord >= 4)
					{
						isActive = false;
						uCoord = 0;
						vCoord = 0;
					}
				}
				coordTime = 0.0f;
			}
			coordTime += time;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if (isActive)
			renderer.RenderParticle(deviceContext, 1, uCoord, vCoord);
	}
	void Activate(XMFLOAT4 newPosition)
	{
		isActive = true;
		particle.Reset();
		particle.SetGravityEffect(0);
		particle.SetHeight(20);
		particle.SetWidth(20);
		particle.SetColor(color);
		particle.SetLifeSpan(-1.0f);
		uCoord = vCoord = 0;
		renderer.particlePositions.positions[0] = particle.GetPosition();
		elaspedTime = 0.0f;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName)
	{
		isActive = false;
		uCoord = 0.0f;
		vCoord = 0.0f;
		coordTime = 0.0f;
		emitterPos = Pos;
		Particle tempParticle;
		tempParticle.SetGravityEffect(0);
		tempParticle.SetHeight(1);
		tempParticle.SetWidth(1);
		particle = tempParticle;
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = &particle;
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particlecolor_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particleanim_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			XMFLOAT4 velocity;
			velocity.x = 0;
			velocity.y = RandFloat(-2, 2);
			velocity.z = 0;
			particle.Update(time, velocity);
			renderer.particlePositions.positions[0] = particle.GetPosition();
			if (coordTime > 0.04f)
			{
				uCoord += 1.0f;
				if (uCoord >= 4)
				{
					uCoord = 0;
					vCoord += 1.0f;
					if (vCoord >= 4)
					{
						isActive = false;
						uCoord = 0;
						vCoord = 0;
					}
				}
				coordTime = 0.0f;
			}
			coordTime += time;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if (isActive)
			renderer.RenderParticle(deviceContext, 1, uCoord, vCoord);
	}
	void Activate(XMFLOAT4 newPosition, XMFLOAT4 newColor)
	{
		isActive = true;
		particle.Reset();
		particle.SetGravityEffect(0);
		particle.SetHeight(5);
		particle.SetWidth(5);
		particle.SetColor(newColor);
		particle.SetLifeSpan(-1.0f);
		uCoord = vCoord = 0;
		renderer.particlePositions.positions[0] = particle.GetPosition();
		elaspedTime = 0.0f;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		isActive = false;
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetLifeSpan(0);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = RandFloat(-20, 20);
					velocity.y = RandFloat(-20, 20);
					velocity.z = RandFloat(-20, 20);
					particles[i].Update(time, velocity);
					if (particles[i].GetElaspedTime() <= 0.0f)
					{
						particles[i].SetLifeSpan(3);
					}
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		isActive = true;
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetHeight(1);
			particles[i].SetWidth(1);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
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
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life = 0.0f)
	{
		isActive = false;
		lifeSpan = life;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
				for (int i = 0; i < size; ++i)
				{
					XMFLOAT4 velocity;
					velocity.x = RandFloat(-20, 20);
					velocity.y = RandFloat(-5, 5);
					velocity.z = RandFloat(-20, 20);
					particles[i].Update(time, velocity);
					renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
				}
			else
				isActive = false;
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	void Activate(float newLife, XMFLOAT4 newPosition)
	{
		isActive = true;
		for (int i = 0; i < size; ++i)
		{
			particles[i].Reset();
			particles[i].SetGravityEffect(0);
			particles[i].SetHeight(1);
			particles[i].SetWidth(1);
			renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
		}
		elaspedTime = 0.0f;
		lifeSpan = newLife;
		emitterPos = newPosition;
		renderer.worldMatrix = XMMatrixTranslation(emitterPos.x, emitterPos.y, emitterPos.z);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
private:
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	float lifeSpan;
	bool isActive;
};

class MovementEmitter
{
public:
	void Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName)
	{
		isActive = false;
		emitterPos = Pos;
		size = amountOfParticles;
		particles.resize(size);
		for (int i = 0; i < size; ++i)
		{
			Particle particle;
			particle.SetGravityEffect(0);
			particle.SetHeight(1);
			particle.SetWidth(1);
			particles[i] = particle;
		}
		renderer.CreateConstantBuffer(renderer.particleCBuff, Device, sizeof(ParticleConstantBuffer));
		renderer.CreateConstantBuffer(renderer.particlePosCBuff, Device, sizeof(ParticlePositionConstantBuffer));
		renderer.particleToRender = particles.data();
		renderer.CreateVertexBuffer(Device);
		ADUtils::SHADER shader = { 0 };
		strcpy_s(shader.vshader, "files\\shaders\\particle_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\particle_ps.hlsl");
		strcpy_s(shader.gshader, "files\\shaders\\particle_gs.hlsl");
		renderer.CreateShadersAndInputLayout(Device, shader);
		renderer.CreateTexture(Device, textureName);
		renderer.worldMatrix = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	}
	void UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
	{
		emitterPos = movementPos;
		if (isActive)
		{
			renderer.particleConstants.ViewMatrix = view;
			renderer.particleConstants.camPos = camPos;
			renderer.particleConstants.ProjectionMatrix = projection;
			renderer.particleConstants.Time = { time, 0,0,0 };
			elaspedTime += time;
			for (int i = 0; i < size; ++i)
			{
				XMFLOAT4 velocity;
				velocity.x = RandFloat(-20, 20);
				velocity.y = RandFloat(-2, 2);
				velocity.z = RandFloat(-20, 20);
				particles[i].Update(time, velocity, emitterPos, 0.5f);
				renderer.particlePositions.positions[i % numParticles] = particles[i].GetPosition();
			}
		}
		else
		{
			for (int i = 0; i < size; ++i)
			{
				particles[i].Reset();
				particles[i].SetGravityEffect(0);
				particles[i].SetHeight(1);
				particles[i].SetWidth(1);
			}
		}
	}
	void RenderParticles(ID3D11DeviceContext* deviceContext)
	{
		if (isActive)
			renderer.RenderParticle(deviceContext, size);
	}
	XMFLOAT4 GetPosition()
	{
		return emitterPos;
	}
	void SetMovementPosition(XMFLOAT4 movePos)
	{
		movementPos = movePos;
	}
	void SetActive(bool active)
	{
		isActive = active;
	}
private:
	bool isActive;
	ParticleRenderer renderer;
	vector<Particle> particles;
	int size;
	XMFLOAT4 emitterPos;
	float elaspedTime = 0.0f;
	XMFLOAT4 movementPos = XMFLOAT4(-100, -100, -100, -100);
};