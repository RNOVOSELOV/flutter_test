#pragma once

#include <string>

using namespace std;
using namespace filesystem;

struct Node;

class IModel {

public:
	IModel();
	virtual ~IModel() = default;

	virtual void startExplore() = 0;
	virtual const vector<pair<path, int>>& getIncludeFilesFreq() = 0;
	virtual const vector <Node*>& getTreeNodes() = 0;

	bool setSourceFilesDirectory(std::string);
	void setSourceDirectory(string);
	void setSourceDirectory(const path&);
	string getSourseDirectory() { return sourcePath.u8string(); }

protected:
	list<path> getIncludeDirs() { return includeDirs; }
	path getSourcePath() { return sourcePath;  }
	bool isValidPath(const path& p, bool checkIsFile = false);	// Проверка path на валидность

private:
	path sourcePath;											// каталог либо файл для которого строится дерево
	list<path> includeDirs;										// список директорий где смотрим библиотечные инклюды 
};