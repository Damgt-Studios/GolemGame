#include "pch.h"
#include "ADParticles.h"


Particle::Particle()
{
	attributes.position = { 0, 0, 0, 1 };
	attributes.color = { 1, 0, 0, 1 };
	velocity = { 0, 0, 0, 1 };
	gravityEffect = 0;
	lifeSpan = RandFloat(0, 3);
	width = 0.5f;
	height = 0.5f;
}

void Particle::Update(double time, XMFLOAT4 newVelocity, XMFLOAT4 newPosition)
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
			lifeSpan = RandFloat(0, 3);
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

XMFLOAT4 Particle::GetPosition()
{
	return attributes.position;
}

ParticleAttributes* Particle::GetAttributes()
{
	return &attributes;
}

float Particle::GetWidth()
{
	return width;
}

float Particle::GetHeight()
{
	return height;
}

float Particle::GetElaspedTime()
{
	return elaspedTime;
}

float Particle::GetLifeSpan()
{
	return lifeSpan;
}

void Particle::SetPosition(XMFLOAT4 position)
{
	attributes.position = position;
}

void Particle::SetColor(XMFLOAT4 color)
{
	attributes.color = color;
}

void Particle::SetGravityEffect(float gEffect)
{
	gravityEffect = gEffect;
}

void Particle::SetWidth(float newWidth)
{
	width = newWidth;
}

void Particle::SetHeight(float newHeight)
{
	height = newHeight;
}

void Particle::SetLifeSpan(float _life)
{
	lifeSpan = _life;
}

void Particle::Reset()
{
	attributes.position = { 0, 0, 0, 1 };
	attributes.color = { 1, 0, 0, 1 };
	velocity = { 0, 0, 0, 0 };
	gravityEffect = 0;
	lifeSpan = RandFloat(0, 3);
	width = 0.5f;
	height = 0.5f;
}

ParticleRenderer::ParticleRenderer()
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

void ParticleRenderer::CreateConstantBuffer(ComPtr<ID3D11Buffer>& ConstantBuffer, ID3D11Device* Device, int SizeOfConstantBuffer)
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

void ParticleRenderer::CreateVertexBuffer(ID3D11Device* Device)
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

void ParticleRenderer::CreateShadersAndInputLayout(ID3D11Device* Device, ADUtils::SHADER& shader)
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

void ParticleRenderer::CreateTexture(ID3D11Device* Device, const wchar_t* Filename)
{
	HRESULT hr = CreateDDSTextureFromFile(Device, Filename, nullptr, &textureResourceView);
}

void ParticleRenderer::RenderParticle(ID3D11DeviceContext* DeviceContext, int InstanceAmount, float u, float v)
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

void FountainEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void FountainEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void FountainEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
		renderer.RenderParticle(deviceContext, size);
}

void FountainEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 FountainEmitter::GetPosition()
{
	return emitterPos;
}

void SpreadEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void SpreadEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void SpreadEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
		renderer.RenderParticle(deviceContext, size);
}

void SpreadEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 SpreadEmitter::GetPosition()
{
	return emitterPos;
}

void CylinderEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, float radius, const wchar_t* textureName)
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

void CylinderEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void CylinderEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
}

void CylinderEmitter::Activate(float newLife, XMFLOAT4 newPosition, float newRadius)
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

XMFLOAT4 CylinderEmitter::GetPosition()
{
	return emitterPos;
}

float CylinderEmitter::GetRadius()
{
	return emitterRadius;
}

void BigCloudEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void BigCloudEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void BigCloudEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
		renderer.RenderParticle(deviceContext, size);
}

void BigCloudEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 BigCloudEmitter::GetPosition()
{
	return emitterPos;
}

void SmallCloudEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void SmallCloudEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void SmallCloudEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
		renderer.RenderParticle(deviceContext, size);
}

void SmallCloudEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 SmallCloudEmitter::GetPosition()
{
	return emitterPos;
}

void UpwardCloudEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void UpwardCloudEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void UpwardCloudEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	{
		if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
			renderer.RenderParticle(deviceContext, size);
	}
}

void UpwardCloudEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 UpwardCloudEmitter::GetPosition()
{
	return emitterPos;
}

bool UpwardCloudEmitter::GetActive()
{
	return isActive;
}

void OutwardCloudEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void OutwardCloudEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void OutwardCloudEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
		renderer.RenderParticle(deviceContext, size);
}

void OutwardCloudEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 OutwardCloudEmitter::GetPosition()
{
	return emitterPos;
}

void LongForwardCloudEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void LongForwardCloudEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void LongForwardCloudEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
		renderer.RenderParticle(deviceContext, size);
}

void LongForwardCloudEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 LongForwardCloudEmitter::GetPosition()
{
	return emitterPos;
}

void WaveEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void WaveEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void WaveEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
		renderer.RenderParticle(deviceContext, size);
}

void WaveEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 WaveEmitter::GetPosition()
{
	return emitterPos;
}

void AnimSpreadEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void AnimSpreadEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void AnimSpreadEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if (elaspedTime < lifeSpan || lifeSpan <= 0.0f)
		renderer.RenderParticle(deviceContext, size, uCoord, vCoord);
}

void AnimSpreadEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 AnimSpreadEmitter::GetPosition()
{
	return emitterPos;
}

void HealthEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, float radius, const wchar_t* textureName) {
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

void HealthEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos) 
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

void HealthEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
		renderer.RenderParticle(deviceContext, size);
}

void HealthEmitter::Activate(float newLife, XMFLOAT4 newPosition, float newRadius)
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

XMFLOAT4 HealthEmitter::GetPosition()
{
	return emitterPos;
}

float HealthEmitter::GetRadius()
{
	return emitterRadius;
}

void EssenceEmitter::Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName)
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

void EssenceEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void EssenceEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if (isActive)
		renderer.RenderParticle(deviceContext, 1);
}

void EssenceEmitter::Activate(XMFLOAT4 newPosition)
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

XMFLOAT4 EssenceEmitter::GetPosition()
{
	return emitterPos;
}

void BloodEmitter::Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName)
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

void BloodEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void BloodEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if (isActive)
		renderer.RenderParticle(deviceContext, 1, uCoord, vCoord);
}

void BloodEmitter::Activate(XMFLOAT4 newPosition)
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

XMFLOAT4 BloodEmitter::GetPosition()
{
	return emitterPos;
}

void BigElementalPuffEmitter::Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName, XMFLOAT4 Color)
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

void BigElementalPuffEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void BigElementalPuffEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if (isActive)
		renderer.RenderParticle(deviceContext, 1, uCoord, vCoord);
}

void BigElementalPuffEmitter::Activate(XMFLOAT4 newPosition)
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

XMFLOAT4 BigElementalPuffEmitter::GetPosition()
{
	return emitterPos;
}

void SmallElementalPuffEmitter::Initialize(ID3D11Device* Device, XMFLOAT4 Pos, const wchar_t* textureName)
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

void SmallElementalPuffEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void SmallElementalPuffEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if (isActive)
		renderer.RenderParticle(deviceContext, 1, uCoord, vCoord);
}

void SmallElementalPuffEmitter::Activate(XMFLOAT4 newPosition, XMFLOAT4 newColor)
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

XMFLOAT4 SmallElementalPuffEmitter::GetPosition()
{
	return emitterPos;
}

void FireballEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void FireballEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void FireballEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
		renderer.RenderParticle(deviceContext, size);
}

void FireballEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 FireballEmitter::GetPosition()
{
	return emitterPos;
}

void IronSkinEmitter::Initialize(ID3D11Device* Device, int amountOfParticles, XMFLOAT4 Pos, const wchar_t* textureName, float life)
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

void IronSkinEmitter::UpdateParticles(float time, XMFLOAT4X4& view, XMFLOAT4X4& projection, XMFLOAT4& camPos)
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

void IronSkinEmitter::RenderParticles(ID3D11DeviceContext* deviceContext)
{
	if ((elaspedTime < lifeSpan || lifeSpan <= 0.0f) && isActive)
		renderer.RenderParticle(deviceContext, size);
}

void IronSkinEmitter::Activate(float newLife, XMFLOAT4 newPosition)
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

XMFLOAT4 IronSkinEmitter::GetPosition()
{
	return emitterPos;
}