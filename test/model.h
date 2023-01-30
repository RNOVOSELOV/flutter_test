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
	path sourceDir;							// ������� - ��������� ����� � ������� ������� ���������
	list<path> includeDirs;					// ������ ���������� ��� ������� ������������ ������� 
	vector<string> validExtensions;

	vector<pair<path, int>> includeFiles;	// ������ �������� � �������� �������������
	vector <Node*> nodes;					// ������� ������������ ��� ������� ����� � ���������� 

    string getHeaderFileName (const string rawHeader, const char firstSymbol, const char secondSymbol);	// ������� ������ �������, ��������� ����� �����
	void cleanNodeTree(Node* node);			// �������� ���� ������

	void makeFilesList();					// ����� ������
	void makeRootTreeNode(const path& p);	// �������� ��� ���� ��� ������
	void makeTree(Node* rootNode, const path& p);	
	void makeTree(const Node * rootNode, Node* parentNode, const path& p, bool calculateIncludes = true);	// �������� ���� ������
	TreeSheetState isNodeWithPathExist(const Node* parentNode, const path& p);	// ����� � ������ ������������ �� ����� ���� (������ �� ����������� ��������)

	pair <LineRegExpStatus, string> parceFileHeaderLine(string line);	// ������� ������ �� ������ �� ���������� ���������
	void addIncludeFile(path path);										// ���������� ������� � ������
	void sortIncludes();												// ���������� ��������
	bool isValidPath(const path& p, bool checkIsFile = false);			// �������� path �� ����������
};
