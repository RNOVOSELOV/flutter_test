#include <iostream>

#include "consoleView.h"
#include "node.h"

ConsoleView::~ConsoleView()
{

}

void ConsoleView::showWelcome(const string directory)
{
	cout << "Welcome to sources directory explorer!\nWould to see statistic of: " << directory << " ?\n>>[y/n]";
	char value;
	cin >> value;
	
	startExploring(value);
}

void ConsoleView::showMessage(const string message)
{
	cout << message << endl;
}

void ConsoleView::startExploring(char value)
{
	notifyPresenter(&IViewEvents::onWelcomeKeyPressed, value);
}

void ConsoleView::showSourceTreeStructure(const vector <Node*>& trees)
{
	for (auto node : trees)
	{
		cout << endl;
		printNodeTree(node);
	}
}

void ConsoleView::printNodeTree(const Node* node, const string prefix)
{
	if (node == nullptr)
	{
		return;
	}
	string notFoundSymbol = node->isFoundOnFilesystem ? "" : "DEL";
	string duplicateSymbol = node->isDuplicate ? "REP" : "";
	cout <<setw(3) << notFoundSymbol << " " << setw(3) << duplicateSymbol << " " << prefix << node->nodePath.filename().u8string() << endl;

	if (prefix.length() == 0 && node->childs.size() == 0)
	{
		cout << "        " << "<include list is empty>" << endl;
	}
	for (auto child : node->childs)
	{
		printNodeTree(child, prefix + "...");
	}
}

void ConsoleView::showIncludeFrequencies(const vector<pair<std::filesystem::path, int>>& includes)
{
	cout << endl;
	for (auto p : includes)
	{
		cout << p.second << "\t" << p.first.filename().u8string()  << endl;
	}
}