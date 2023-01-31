#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <iostream>
#include "model.h"
#include "node.h"

Model::Model()
{
	sourcePath.clear();
	validExtensions.push_back(".h");
	validExtensions.push_back(".hpp");
	validExtensions.push_back(".cpp");
}

Model::~Model()
{
	for (auto it = nodes.begin(); it != nodes.end(); it++)
	{
		cleanNodeTree(*it);
	}
	nodes.clear();
}

void Model::cleanNodeTree(Node* node) 
{
	if (node != nullptr) {
		for (auto it = node->childs.begin(); it != node->childs.end(); it++)
		{
			cleanNodeTree(*it);
		}
		node->childs.clear();
		delete node;
	}
}

void Model::setSourceDirectory(string filePath)
{
	const path directory{ filePath };
	if (exists(directory))
	{
		setSourceDirectory(canonical(directory));
	}
}

void Model::setSourceDirectory(const path& directory)
{
	sourcePath = directory;
}

bool Model::setSourseFilesDirestory(string directory)
{
	const path sourcesDirectory{ directory };
	if (isValidPath(sourcesDirectory))
	{
		includeDirs.push_back(canonical(sourcesDirectory));
		return true;
	}
	return false;
}

AnalyzeResult Model::startExplore()
{
	startMakeFilesTree();
	if (includeFiles.size() != 0)
	{
		sortIncludes();
	}
	return AnalyzeResult::successful;
}

void Model::startMakeFilesTree()
{
	if (is_regular_file(sourcePath))
	{
		makeRootTreeNode(sourcePath);
	}
	else if (is_directory (sourcePath))
	{
		for (directory_entry entry : recursive_directory_iterator(sourcePath))
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

pair <LineRegExpStatus, string> Model::parceFileHeaderLine(string line)
{
	regex directiveRegEx{ "\\s*\\#[\\w\\W]*" };
	regex includeRegEx { "\\s*\\#include\\s*<[a-zA-Z0-9]+[.]?[h]?>\\s*" };
	regex localRegEx { "\\s*\\#include\\s*\"[a-zA-Z0-9]+[.]?[h]?\"\\s*" };
	regex emptyString{ "^\\s*$" };
	regex commentedLineRegEx{ "\\s*\\/\\/\\s*[\\w\\W]*" };

	if (regex_match(line, includeRegEx))
	{
        return pair(LineRegExpStatus::validIncludeHeader, getHeaderFileName(line, '<', '>'));
	}
	else if (regex_match(line, localRegEx))
	{
        return pair(LineRegExpStatus::validLocalHeader, getHeaderFileName(line, '\"', '\"'));
	}
	else if (regex_match(line, emptyString) || regex_match(line, directiveRegEx) || regex_match(line, commentedLineRegEx))
	{
		return pair(LineRegExpStatus::skipHeaderLine, "");
	}
	return pair(LineRegExpStatus::invalidHeader, "");
}

string Model::getHeaderFileName (const string rawHeader, const char firstSymbol, const char secondSymbol)
{
    auto it1 = rawHeader.find_first_of(firstSymbol) +1;
    auto it2 = rawHeader.find_first_of(secondSymbol, it1);
    string str2(it2-it1, '\0');
    copy(rawHeader.begin()+it1, rawHeader.begin()+it2, str2.begin());
    return str2;
}

bool Model::isValidPath(const path& p, bool checkIsFile)
{
	if (!exists(p))
	{
		return false;
	}
	if (checkIsFile && !is_regular_file(p))
	{
		return false;
	}
	return true;
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

void Model::makeTree(const Node* rootNode, Node* parentNode, const path& p, bool calculateIncludes)
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
		auto parceLineResult{ parceFileHeaderLine(s) };
		if (parceLineResult.first == LineRegExpStatus::validIncludeHeader)
		{
			Node* node = new Node();
			node->isDuplicate = false;
			node->isFoundOnFilesystem = false;
			path  includeFilePath;
			bool validFileFound = false;
			for (auto include : includeDirs)
			{
				includeFilePath = include;
				includeFilePath.append(parceLineResult.second);
				if (isValidPath(includeFilePath, true))
				{
					if (calculateIncludes)
					{
						addIncludeFile(includeFilePath);
					}
					validFileFound = true;
					node->nodePath = canonical(includeFilePath);
					node->isFoundOnFilesystem = true;
				}
			}
			if (!validFileFound)
			{
				node->nodePath = parceLineResult.second;
				if (calculateIncludes)
				{
					addIncludeFile(parceLineResult.second);
					cout << "\nWARNING! In " << p.u8string() << ": header \"" << parceLineResult.second << "\" not found in -I options directories!" << endl;
				}
			}
			parentNode->childs.push_back(node);
		}
		else if (parceLineResult.first == LineRegExpStatus::validLocalHeader)
		{
			path includeFilePath = p.parent_path();
			includeFilePath.append(parceLineResult.second);
			if (calculateIncludes)
			{
				addIncludeFile(includeFilePath);
			}

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
			if (valuePathExist)
			{
				node->isDuplicate = true;
			}
			else
			{
				makeTree(rootNode, node, includeFilePath, false);
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
	auto compareValue = parentNode->nodePath.compare(p);
	if (compareValue == 0)
	{
		return true;
	}
	if (parentNode->childs.size() != 0)
	{
		bool oneOfChildIsPath = false;
		auto it = parentNode->childs.begin();
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
