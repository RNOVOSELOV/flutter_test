#pragma once

#include <filesystem>
#include <vector>
#include <iostream>

struct Node
{
	std::filesystem::path nodePath;
	bool isFoundOnFilesystem = { false };
	bool isDuplicate = { false };
	std::vector<Node*> childs;
};
