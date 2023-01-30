#pragma once

#include "iview.h"

class ConsoleView :
    public IView
{
public:
    virtual ~ConsoleView() {}
    virtual void showWelcome(const string directory);
    virtual void showMessage(const string);

    virtual void showSourceTreeStructure(const vector <Node*>&);
    virtual void showIncludeFrequencies(const vector<pair<std::filesystem::path, int>>&);

    void startExploring(char value);

private:
    void printNodeTree(const Node* node, const string prefix = "");
};

