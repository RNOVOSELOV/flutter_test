#pragma once

#include <string>

struct Node;

class IModel {

public:
	IModel();
	virtual ~IModel() = default;

	virtual void startExplore() = 0;
	virtual const std::vector<std::pair<std::string, int>>& getIncludeFilesFreq() = 0;
	virtual const std::vector <Node*>& getTreeNodes() = 0;

	bool setSourceFilesDirectory(std::string);
	bool setSourceDirectory(std::string);
	bool setSourceDirectory(const std::filesystem::path&);
	std::string getSourseDirectoryName() { return sourcePath.u8string(); }
	bool setProjectName(std::string param);
	std::filesystem::path getProjectName() { return projectName; }

protected:
	std::list<std::filesystem::path> getIncludeDirs() { return includeDirs; }
	std::filesystem::path getSourcePath() { return sourcePath;  }
	bool isValidPath(const std::filesystem::path& p, bool checkIsFile = false);	// Проверка path на валидность

private:
	std::filesystem::path sourcePath;											// каталог либо файл для которого строится дерево
	std::list<std::filesystem::path> includeDirs;										// список директорий где смотрим библиотечные инклюды 
	std::filesystem::path projectName;
};