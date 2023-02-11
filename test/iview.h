#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "iviewEvents.h"
#include "viewEventsNotifier.h"

struct Node;

class IView: public ViewEventsNotifier<IViewEvents>  {
public:
	virtual ~IView() = default;
	virtual void showWelcome(const std::string directory) = 0;
	virtual void showMessage(const std::string) = 0;

	virtual void showSourceTreeStructure (const std::vector <Node *>&) = 0;
	virtual void showIncludeFrequencies(const std::vector<std::pair<std::string, int>>&) = 0;
};
