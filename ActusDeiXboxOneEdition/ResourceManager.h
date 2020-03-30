#pragma once

#include <string>
#include <vector>



class ResourceManager
{
public:
	ResourceManager() = default;
	~ResourceManager() = default;

	// Utils
	std::string generateUUIDV4();

private:
	const std::string CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
};

// Note: Update uuid to use long long