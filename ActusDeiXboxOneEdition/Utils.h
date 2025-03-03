#pragma once

#include <string>
#include <fstream>

#include "Types.h"

using namespace ADResource::ADRenderer;

namespace ADUtils
{
	struct SHADER
	{
		char vshader[64];
		char pshader[64];
		char gshader[64];

		bool wireframe = false;
	};

	static std::string READ_PATH("");
	static std::string WRITE_PATH("");

	static const char SHADER_ENTRY_POINT[] = "main";
	static const char SHADER_MODEL_PS[] = "ps_4_0";
	static const char SHADER_MODEL_VS[] = "vs_4_0";
	static const char SHADER_MODEL_GS[] = "gs_4_0";

	void InitializeFileSystem();

	// Model data
	//void LoadWobjectMesh(const char* meshname, Model& model, ComPtr<ID3D11Device1> device, SHADER& shader);
	void LoadStaticMesh(const char* meshname, SimpleStaticModel& model, ComPtr<ID3D11Device1> device, SHADER& shader);
	void LoadAnimatedMesh(const char* meshname, SimpleAnimModel& model, std::vector<std::string> animations, ComPtr<ID3D11Device1> device, SHADER& shader);

	//void LoadTextures(Header& header, Model& model, ComPtr<ID3D11Device1> device);

	void LoadTextures(std::string filepath, SimpleModel* model, ComPtr<ID3D11Device1> device, std::unordered_map<std::string, ADTexture>& textures);
	void LoadUITextures(std::string _fileName, ID3D11ShaderResourceView** _srv, ComPtr<ID3D11Device1> _dev);
	std::wstring GetTexturePath(const char* textName);
};