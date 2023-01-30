#pragma once

#include <filesystem>
#include <vector>
#include <iostream>

struct Node
{
	std::filesystem::path nodePath;
	bool isFoundOnFilesystem;
	bool isDuplicate;
	std::vector<Node*> childs;

	~Node()
	{
		cout << "Removed node: " << nodePath.u8string() << endl;
	}
};
