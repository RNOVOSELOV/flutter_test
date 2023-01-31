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
	virtual pair <LineRegExpStatus, string> validationAndParcingHeaderLine(string line) = 0;	// ������� ������ �� ������ �� ���������� ���������

	void addValidExtension(string ext) { validExtensions.push_back(ext); }

private:
	vector<string> validExtensions;
	vector<pair<path, int>> includeFiles;	// ������ �������� � �������� �c�����������
	vector <Node*> nodes;					// ������� ������������ ��� ������� ����� � ���������� 

	void cleanNodeTree(Node* node);				// �������� ���� ������

	void startMakeFilesTree();					// ����� ������
	void makeRootTreeNode(const path& p);		// �������� ��� ���� ��� ������
	void makeTree(Node* rootNode, const path& p);	
	void makeTree(const Node * rootNode, Node* parentNode, const path& p, bool calculateIncludes = true);	// �������� ���� ������
	bool isNodeWithPathExist(const Node* parentNode, const path& p);	// ����� � ������ ������������ �� ����� ���� (������ �� ����������� ��������)

	void addIncludeFile(path path);										// ���������� ������� � ������
	void sortIncludes();												// ���������� ��������
};
