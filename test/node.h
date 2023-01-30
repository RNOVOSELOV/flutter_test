#pragma once

#include <filesystem>
#include <vector>

struct Node
{
	std::filesystem::path nodePath;
	bool isFoundOnFilesystem;
	bool isDuplicate;
	std::vector<Node*> childs;
};
