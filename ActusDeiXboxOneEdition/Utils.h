#pragma once

#include <string>
#include <fstream>

#include "Types.h"

using namespace ADResource::ADRenderer;

namespace ADUtils
{
	static std::string READ_PATH("");
	static std::string WRITE_PATH("");

	void InitializeFileSystem();

	// Model data
	void LoadWobjectMesh(const char* meshname, Model& model, ComPtr<ID3D11Device1> device);
	void LoadTextures(Header& header, Model& model, ComPtr<ID3D11Device1> device);
};