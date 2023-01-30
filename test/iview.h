#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "iviewEvents.h"
#include "viewEventsNotifier.h"

using namespace std;
struct Node;

class IView: public ViewEventsNotifier<IViewEvents>  {
public:
	virtual ~IView() = default;
	virtual void showWelcome(const string directory) = 0;
	virtual void showMessage(const string) = 0;

	virtual void showSourceTreeStructure (const vector <Node *>&) = 0;
	virtual void showIncludeFrequencies(const vector<pair<std::filesystem::path, int>>&) = 0;
};
