#include "pchgame.h"
#include "GameUtilities.h"


ADResource::ADGameplay::Spyro* GameUtilities::LoadSpyroFromModelFile(std::string modelname, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADResource::ADGameplay::Spyro* temp = new ADResource::ADGameplay::Spyro;

	ADUtils::SHADER shader = { 0 };
	strcpy_s(shader.vshader, "files\\shaders\\spyro_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\spyro_ps.hlsl");

	AD_ULONG id = ResourceManager::InitializePBRModel(modelname, position, scale, rotation, shader);
	temp->SetMeshID(id);

	return temp;
}