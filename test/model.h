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
	virtual pair <LineRegExpStatus, string> validationAndParcingHeaderLine(string line) = 0;	// ������� ������ �� ������ �� ���������� ���������

	void addValidExtension(string ext) { validExtensions.push_back(ext); }

private:
	vector<string> validExtensions;
	vector <pair<string, int>> includes;	// ������ �������� � �������� �c�����������
	vector<path> uniqueIncludes;
	vector <Node*> nodes;					// ������� ������������ ��� ������� ����� � ���������� 

	void cleanNodeTree(Node* node);				// �������� ���� ������

	void startMakeFilesTree();					// ����� ������
	void makeRootTreeNode(const path& p);		// �������� ��� ���� ��� ������
	void makeTree(Node* rootNode, const path& p);	
	void makeTree(const Node * rootNode, Node* parentNode, const path& p);	// �������� ���� ������
	bool isNodeWithPathExist(const Node* parentNode, const path& p);	// ����� � ������ ������������ �� ����� ���� (������ �� ����������� ��������)

	void addIncludeFile(path path);										// ���������� ������� � ������
	void sortIncludes(vector <pair<string, int>>&);
	path createUniqueIncludePath(const path&, const string&, const string&);
};
