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
	pair <LineRegExpStatus, string> validationAndParcingHeaderLine(string line);	// ������� ������ �� ������ �� ���������� ���������

private:
	vector<string> validExtensions;
	vector<pair<path, int>> includeFiles;	// ������ �������� � �������� �c�����������
	vector <Node*> nodes;					// ������� ������������ ��� ������� ����� � ���������� 

    string getHeaderFileName (const string rawHeader, const char firstSymbol, const char secondSymbol);	// ������� ������ �������, ��������� ����� �����
	void cleanNodeTree(Node* node);				// �������� ���� ������

	void startMakeFilesTree();					// ����� ������
	void makeRootTreeNode(const path& p);		// �������� ��� ���� ��� ������
	void makeTree(Node* rootNode, const path& p);	
	void makeTree(const Node * rootNode, Node* parentNode, const path& p, bool calculateIncludes = true);	// �������� ���� ������
	bool isNodeWithPathExist(const Node* parentNode, const path& p);	// ����� � ������ ������������ �� ����� ���� (������ �� ����������� ��������)

	void addIncludeFile(path path);										// ���������� ������� � ������
	void sortIncludes();												// ���������� ��������
};
