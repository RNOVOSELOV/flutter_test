#pragma once

#include <filesystem>
#include <vector>
#include <list>

#include "imodel.h"

enum class LineRegExpStatus {
	skipHeaderLine,
	validLocalHeader,	
	validIncludeHeader,	
	invalidHeader	
};

class Model: public IModel
{
public:
	Model();
	~Model();

	virtual void startExplore();
	virtual const vector<pair<path, int>>& getIncludeFilesFreq() { return includeFiles; }
	virtual const vector <Node*>& getTreeNodes() { return nodes; }

protected:
	virtual pair <LineRegExpStatus, string> validationAndParcingHeaderLine(string line) = 0;	// Парсинг строки из хедера на отсновании регулярки

	void addValidExtension(string ext) { validExtensions.push_back(ext); }

private:
	vector<string> validExtensions;
	vector<pair<path, int>> includeFiles;	// список инклюдов с частотой иcпользования
	vector <Node*> nodes;					// деревья зависимостей для каждого файла в директории 

	void cleanNodeTree(Node* node);				// Удаление узла дерева

	void startMakeFilesTree();					// Обход файлов
	void makeRootTreeNode(const path& p);		// Создание рут узла для дерева
	void makeTree(Node* rootNode, const path& p);	
	void makeTree(const Node * rootNode, Node* parentNode, const path& p, bool calculateIncludes = true);	// Создание узла дерева
	bool isNodeWithPathExist(const Node* parentNode, const path& p);	// Поиск в дереве присутствует ли такой узел (защита от циклических вложений)

	void addIncludeFile(path path);										// Добавление инклюда в список
	void sortIncludes();												// Сортировка инклюдов
};
