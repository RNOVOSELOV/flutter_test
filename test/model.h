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

using std::string;
using std::vector;
using std::pair;
using std::filesystem::path;

class Model: public IModel
{
public:
	Model();
	~Model();

	virtual void startExplore();
	virtual const vector<pair<string, int>>& getIncludeFilesFreq();
	virtual const vector <Node*>& getTreeNodes() { return nodes; }

protected:
	virtual pair <LineRegExpStatus, string> validationAndParcingHeaderLine(string line) = 0;	// Парсинг строки из хедера на отсновании регулярки

	void addValidExtension(string ext) { validExtensions.push_back(ext); }

private:
	vector<string> validExtensions;
	vector <pair<string, int>> includes;	// список инклюдов с частотой иcпользования
	vector<path> uniqueIncludes;
	vector <Node*> nodes;					// деревья зависимостей для каждого файла в директории 

	void cleanNodeTree(Node* node);				// Удаление узла дерева

	void startMakeFilesTree();					// Обход файлов
	void makeRootTreeNode(const path& p);		// Создание рут узла для дерева
	void makeTree(Node* rootNode, const path& p);	
	void makeTree(const Node * rootNode, Node* parentNode, const path& p);	// Создание узла дерева
	bool isNodeWithPathExist(const Node* parentNode, const path& p);	// Поиск в дереве присутствует ли такой узел (защита от циклических вложений)

	void addIncludeFile(path path);										// Добавление инклюда в список
	void sortIncludes(vector <pair<string, int>>&);
	path createUniqueIncludePath(const path&, const string&, const string&);
};
