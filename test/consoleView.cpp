#include <iostream>

#include "consoleView.h"
#include "node.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;

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

void ConsoleView::showSourceTreeStructure(const std::vector <Node*>& trees)
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
	cout << " " << std::setw(3) << duplicateSymbol << " " << std::setw(3) << notFoundSymbol << " " << prefix << node->nodePath.filename().u8string() << endl;

	if (prefix.length() == 0 && node->childs.size() == 0)
	{
		cout << "         " << "<include list is empty>" << endl;
	}
	for (auto child : node->childs)
	{
		printNodeTree(child, prefix + "...");
	}
}

void ConsoleView::showIncludeFrequencies(const std::vector<std::pair<string, int>>& includes)
{
	cout  << endl;
	for (auto p : includes)
	{
		cout << " " << p.second << "\t" << p.first << endl;
	}
}