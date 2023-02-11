#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <iostream>
#include "model.h"
#include "node.h"

Model::Model()
{
	
}

Model::~Model()
{
	for (auto it{ nodes.begin() }; it != nodes.end(); it++)
	{
		cleanNodeTree(*it);
	}
	nodes.clear();
}

void Model::cleanNodeTree(Node* node) 
{
	if (node != nullptr) {
		for (auto it{ node->childs.begin() }; it != node->childs.end(); it++)
		{
			cleanNodeTree(*it);
		}
		node->childs.clear();
		delete node;
	}
}

void Model::startExplore()
{
	includes.clear();
	startMakeFilesTree();
}

const vector<pair<string, int>>& Model::getIncludeFilesFreq()
{
	for (auto unique : uniqueIncludes)
	{
		bool addToInclude = true;
		for (auto include {includes.begin ()}; include != includes.end(); ++include)
		{
			if (unique.filename().u8string() == (*include).first)
			{
				addToInclude = false;
				(*include).second = (*include).second + 1;
				break;
			}
		}
		if (addToInclude)
		{
			includes.push_back(std::pair(unique.filename().u8string(), 1));
		}
	}
	sortIncludes(includes);
	return includes;
}

void Model::startMakeFilesTree()
{
	if (is_regular_file(getSourcePath()))
	{
		makeRootTreeNode(getSourcePath());
	}
	else if (is_directory (getSourcePath()))
	{
		for (std::filesystem::directory_entry entry : std::filesystem::recursive_directory_iterator(getSourcePath()))
		{
			if (entry.is_regular_file())
			{
				if (any_of(validExtensions.begin(), validExtensions.end(), 
					[&entry](auto& item)->bool { return item == entry.path().extension(); })
					)
				{
					auto p{ entry.path() };
					makeRootTreeNode(p);
				}
			}
		}
	}
	
}

void Model::sortIncludes(vector <pair<string, int>>& includes)
{
	std::sort(includes.begin(), includes.end(),
		[](pair <string, int>& a, pair <string, int>& b)
		{
			if (a.second == b.second)
			{
				return a.first < b.first;
			}
	return a.second > b.second;
		});
}

void Model::addIncludeFile(path path)
{
	for (auto tempPath : uniqueIncludes)
	{
		if (path.compare(tempPath) == 0)
		{
			return;
		}
	}
	uniqueIncludes.push_back(path);
}

void Model::makeRootTreeNode(const path& p)
{
	Node* rootNode = new Node();
	rootNode->isFoundOnFilesystem = true;
	rootNode->nodePath = p;
	rootNode->isDuplicate = false;
	nodes.push_back(rootNode);
	makeTree(rootNode, p);
}

void Model::makeTree(Node* rootNode, const path& p)
{
	makeTree(rootNode, rootNode, p);
}

void Model::makeTree(const Node* rootNode, Node* parentNode, const path& p)
{
	using std::regex;

	regex startCommentRegEx{ "[\\w\\W]*/\\* [\\w\\W] * " };
	regex endCommentRegEx{ "[\\w\\W]*\\*/[\\w\\W]*" };

	std::ifstream fstream{ p.c_str() };
	string s;
	auto isCommentted{ false };

	for (; getline(fstream, s);)
	{
		if (regex_match(s, startCommentRegEx))
		{
			isCommentted = true;
			continue;
		}
		if (isCommentted)
		{
			if (regex_match(s, endCommentRegEx))
			{
				isCommentted = false;
				continue;
			}
			continue;
		}
		auto parceLineResult{ validationAndParcingHeaderLine(s) };
		if (parceLineResult.first == LineRegExpStatus::validIncludeHeader)
		{
			Node* node = new Node();
			node->isDuplicate = false;
			node->isFoundOnFilesystem = false;
			path  includeFilePath;
			bool validFileFound = false;
			for (auto include : getIncludeDirs())
			{
				includeFilePath = include;
				includeFilePath.append(parceLineResult.second);
				if (isValidPath(includeFilePath, true))
				{
					validFileFound = true;
					node->nodePath = canonical(includeFilePath);
					node->isFoundOnFilesystem = true;
					break;
				}
			}
			if (!validFileFound)
			{
				node->nodePath = parceLineResult.second;
				includeFilePath = parceLineResult.second;
			}
			parentNode->childs.push_back(node);
			addIncludeFile(createUniqueIncludePath(p.parent_path(), p.filename().u8string(), parceLineResult.second));
		}
		else if (parceLineResult.first == LineRegExpStatus::validLocalHeader)
		{
			path includeFilePath = p.parent_path();
			includeFilePath.append(parceLineResult.second);
			Node* node = new Node();
			node->isFoundOnFilesystem = false;
			node->nodePath = includeFilePath;
			node->isDuplicate = false;

			auto valuePathExist { isNodeWithPathExist(rootNode, includeFilePath) };

			if (isValidPath(includeFilePath, true))
			{
				node->isFoundOnFilesystem = true;
			}
			parentNode->childs.push_back(node);

			addIncludeFile(createUniqueIncludePath(p.parent_path(), p.filename().u8string(), parceLineResult.second));
			if (valuePathExist)
			{
				node->isDuplicate = true;
			}
			else
			{
				makeTree(rootNode, node, includeFilePath);
			}
		}
		else if (parceLineResult.first == LineRegExpStatus::skipHeaderLine)
		{
			continue;
		}
		else if (parceLineResult.first == LineRegExpStatus::invalidHeader)
		{
			// Заканчиваем обработку этого файла хедеры закончились
			break;
		}
	}
}

path Model::createUniqueIncludePath(const path& parentPath, const string& fileName, const string& includeName)
{
	path uniqueIncludeFilePath{ parentPath };
	uniqueIncludeFilePath.append(fileName);
	uniqueIncludeFilePath.append(includeName);
	return uniqueIncludeFilePath;
}

bool Model::isNodeWithPathExist(const Node* parentNode, const path& p)
{
	auto compareValue{ parentNode->nodePath.compare(p) };
	if (compareValue == 0)
	{
		return true;
	}
	if (parentNode->childs.size() != 0)
	{
		bool oneOfChildIsPath = false;
		auto it{ parentNode->childs.begin() };
		while (it != parentNode->childs.end())
		{
			auto value{ isNodeWithPathExist(*it, p) };
			if (value)
			{
				oneOfChildIsPath = value;
				break;
			}
			it++;
		}
		return oneOfChildIsPath;
	}
	return false;
}
