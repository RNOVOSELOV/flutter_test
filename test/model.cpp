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
	startMakeFilesTree();
	if (includeFiles.size() != 0)
	{
		sortIncludes();
	}
}

void Model::startMakeFilesTree()
{
	if (is_regular_file(getSourcePath()))
	{
		makeRootTreeNode(getSourcePath());
	}
	else if (is_directory (getSourcePath()))
	{
		for (directory_entry entry : recursive_directory_iterator(getSourcePath()))
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

void Model::sortIncludes()
{
	std::sort(includeFiles.begin(), includeFiles.end(),
		[](pair <path, int>& a, pair <path, int>& b)
		{
			if (a.second == b.second)
			{
				return a.first.filename() < b.first.filename();
			}
	return a.second > b.second;
		});
}

void Model::addIncludeFile(path path)
{
	for (int i = 0; i < includeFiles.size(); ++i)
	{
		if (includeFiles[i].first == path)
		{
			++includeFiles[i].second;
			return;
		}
	}
	includeFiles.push_back(pair(path, 1));
}

string Model::getHeaderFileName (const string rawHeader, const char firstSymbol, const char secondSymbol)
{
	auto it1{ rawHeader.find_first_of(firstSymbol) + 1 };
	auto it2{ rawHeader.find_first_of(secondSymbol, it1) };
    string str2(it2-it1, '\0');
    copy(rawHeader.begin()+it1, rawHeader.begin()+it2, str2.begin());
    return str2;
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
	regex startCommentRegEx{ "[\\w\\W]*/\\* [\\w\\W] * " };
	regex endCommentRegEx{ "[\\w\\W]*\\*/[\\w\\W]*" };

	ifstream fstream{ p.c_str() };
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
//					cout << "\nWARNING! In " << p.u8string() << ": header \"" << parceLineResult.second << "\" not found in -I options directories!" << endl;
			}
			parentNode->childs.push_back(node);
			if (!parentPathHaveInAnotherTrees(rootNode, parentNode->nodePath))
			{
				cout << "addInclude " << includeFilePath.u8string();
				addIncludeFile(includeFilePath);
			}
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
	//		cout << "MODEL: " << rootNode->nodePath.filename().u8string() << " " << parentNode->nodePath.filename().u8string() << " " << node->nodePath.filename().u8string() << endl;
			if (!parentPathHaveInAnotherTrees(rootNode, parentNode->nodePath))
			{
				cout << "addInclude " << includeFilePath.u8string()  << endl << endl;
				addIncludeFile(includeFilePath);
			}
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


bool Model::parentPathHaveInAnotherTrees(const Node* excludeRootNode, const path& parentPath)
{
	cout << "START pathHaveInAnotherTrees ex: " << excludeRootNode->nodePath.filename().u8string()<< " " << parentPath.filename().u8string() << endl;
	bool isFound = false;
	
	int op = 0;

	for (auto it{ nodes.begin() }; it != nodes.end(); it++)
	{
		cout << "FOR: " << ++op << " " << nodes.size() << " " << (*it)->nodePath.u8string() << endl;
		if ((*it)->nodePath.compare(excludeRootNode->nodePath) == 0)
		{
			cout << "compare 0 -> continue" << endl;
			continue;
		}
		isFound = isNodeWithPathExist(*it, parentPath);
		cout << "isNodeWithParentAndChildExist " << isFound << endl;
		if (isFound)
		{
			cout << "break TRUE" << isFound << endl;
			break;
		}
	}
	cout << "END FALSE" << isFound << endl;
	return isFound;
}

/*
bool Model::isNodeWithParentAndChildExist(const Node* parentNode, const path& parent, const path& child)
{
	if (parentNode->childs.size() == 0)
	{
		return false;
	}
	auto compareValue{ parentNode->nodePath.compare(parent) };
	cout << "IS: compare " << compareValue << " " << parentNode->nodePath.u8string() << " " << parent.u8string() << endl;
	if (compareValue == 0)
	{
		for (auto i{ parentNode->childs.begin() }; i != parentNode->childs.end(); ++i)
		{
			if ((*i)->nodePath.compare(child) == 0)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		bool found = false;
		auto it{ parentNode->childs.begin() };
		while (it != parentNode->childs.end())
		{
			auto value{ isNodeWithParentAndChildExist(*it, parent, child) };
			if (value)
			{
				found = value;
				break;
			}
			it++;
		}
		return found;
	}
	return false;
}
*/