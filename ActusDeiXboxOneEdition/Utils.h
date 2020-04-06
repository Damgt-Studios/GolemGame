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
	};

	static std::string READ_PATH("");
	static std::string WRITE_PATH("");

	static const char SHADER_ENTRY_POINT[] = "main";
	static const char SHADER_MODEL_PS[] = "ps_4_0";
	static const char SHADER_MODEL_VS[] = "vs_4_0";

	void InitializeFileSystem();

	// Model data
	void LoadWobjectMesh(const char* meshname, Model& model, ComPtr<ID3D11Device1> device, SHADER& shader);
	void LoadTextures(Header& header, Model& model, ComPtr<ID3D11Device1> device);
};