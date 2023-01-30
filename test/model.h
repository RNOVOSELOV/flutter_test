#pragma once

#include <filesystem>
#include <vector>
#include <list>

using namespace std;
using namespace filesystem;

struct Node;

enum class AnalyzeResult {successful, sourceDirError};
enum class LineRegExpStatus {
	skipHeaderLine,
	validLocalHeader,	
	validIncludeHeader,	
	invalidHeader	
};
enum class TreeSheetState { sheetIsPersentInTree, sheetIsMissingInTree, seatchNotCompleted };

class Model
{
public:
	Model();
	~Model();

	void setSourceDirectory(string);
	void setSourceDirectory(const path&);
	string getSourseDirectory() { return sourceDir.u8string(); }

	bool setSourseFilesDirestory(string);
	AnalyzeResult startExplore();

	const vector<pair<path, int>>& getIncludeFilesFreq() { return includeFiles; }
	const vector <Node*>& getTreeNodes() { return nodes; }

private:
	path sourceDir;							// каталог - отправная точка в которой смотрим исходники
	list<path> includeDirs;					// список директорий где смотрим библиотечные инклюды 
	vector<string> validExtensions;

	vector<pair<path, int>> includeFiles;	// список инклюдов с частотой изпользования
	vector <Node*> nodes;					// деревья зависимостей для каждого файла в директории 

    string getHeaderFileName (const string rawHeader, const char firstSymbol, const char secondSymbol);	// Парсинг строки инклюда, получение имени файла
	void cleanNodeTree(Node* node);			// Удаление узла дерева

	void makeFilesList();					// Обход файлов
	void makeRootTreeNode(const path& p);	// Создание рут узла для дерева
	void makeTree(Node* rootNode, const path& p);	
	void makeTree(const Node * rootNode, Node* parentNode, const path& p, bool calculateIncludes = true);	// Создание узла дерева
	TreeSheetState isNodeWithPathExist(const Node* parentNode, const path& p);	// Поиск в дереве присутствует ли такой узел (защита от циклических вложений)

	pair <LineRegExpStatus, string> parceFileHeaderLine(string line);	// Парсинк строки из хедера на отсновании решулярки
	void addIncludeFile(path path);										// Добавление инклюда в список
	void sortIncludes();												// Сортировка инклюдов
	bool isValidPath(const path& p, bool checkIsFile = false);			// Проверка path на валидность
};
