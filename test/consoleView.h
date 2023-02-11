#pragma once

#include "iview.h"

class ConsoleView :
    public IView
{
public:
    virtual ~ConsoleView();
    virtual void showWelcome(const std::string directory);
    virtual void showMessage(const std::string);

    virtual void showSourceTreeStructure(const std::vector <Node*>&);
    virtual void showIncludeFrequencies(const std::vector<std::pair<std::string, int>>&);

    void startExploring(char value);

private:
    void printNodeTree(const Node* node, const std::string prefix = "");
};

