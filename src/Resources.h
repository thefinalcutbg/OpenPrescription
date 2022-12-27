#pragma once
#include <string>
#include <vector>
/*
Ideally these should be used only once in the program lifetime
*/
namespace Resources
{
	std::string fromPath(const char* path);
	std::string citiesJson();
	std::vector<std::string> dbSchema();
};

