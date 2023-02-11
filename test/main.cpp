#include <iostream>
#include "consoleView.h"
#include "dartModel.h"
#include "cppModel.h"
#include "presenter.h"
#include "cppModelFactory.h"
#include "dartModelFactory.h"

// "C:\android\projects\repo\gift_manager\lib\main.dart" -D -I "C:\flutter\packages\flutter\lib" -P "C:\android\projects\repo\gift_manager"
// . -I "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.34.31933\include"

int main(int argc, char * argv[])
{
	std::shared_ptr<IView> view = std::make_shared <ConsoleView>();
    if (argc <= 1)
    {
		view->showMessage("Error! Number of arguments error. Source path isn't setted.\n");
        return 1;
    }

	ModelFactory* factory = nullptr;
	bool isProjectDirectoryNeeded = false;
	for (int i = 1; i < argc; ++i)
	{
		string value = *(argv + i);
		if (value == "-C")
		{
			factory = new CppModelFactory();
		}
		else if (value == "-D")
		{
			factory = new DartModelFactory();
			isProjectDirectoryNeeded = true;
		}
	}
	if (factory == nullptr)
	{
		factory = new CppModelFactory();
	}

	std::shared_ptr<IModel> model = factory->createModel();
	delete factory;
	for (int i = 1; i < argc; ++i)
	{
		string value = *(argv + i);
		if (value == "-I" && i != argc - 1)
		{
			string directory = *(argv + ++i);
			auto isValid{ model->setSourceFilesDirectory(directory) };
			if (!isValid)
			{
				view->showMessage("WARNING! Directory \"" + directory + "\" not exist.\n");
			}
		}
		else if (value == "-P" && i != argc - 1)
		{
			string project = *(argv + ++i);
			model->setProjectName(project);
		}
		else if (value == "-C" || value == "-D")
		{
			continue;
		}
		else
		{
			// Anything parameter without -I prefix
			if (!model->setSourceDirectory(value))
			{
				view->showMessage("WARNING! Incorrect PATH is entered!");
			}
		}
	}

	if (isProjectDirectoryNeeded && model->getProjectName().empty())
	{
		view->showMessage("Error! Need to set progect directory for this language. Use -P parameter.");
		return 2;
	}

	if (model->getSourseDirectoryName().empty())
	{
		view->showMessage("Error! Source's directory not exist on filesystem.\n");
		return 3;
	}

	auto presenter{ std::make_unique<Presenter>(view, model) };
    presenter->startAnalyze();

    return 0;
}
