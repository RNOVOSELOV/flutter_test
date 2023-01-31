#include <iostream>
// #include "hello.h"
#include "consoleView.h"
#include "cppmodel.h"
#include "dartModel.h"
#include "presenter.h"

// "C:\android\projects\repo\gift_manager\lib\main.dart" -I "C:\flutter\packages\flutter\lib"

int main(int argc, char * argv[])
{
	shared_ptr<IView> view = make_shared <ConsoleView>();
    if (argc <= 1)
    {
		view->showMessage("Error! Number of arguments error. Source path isn't setted.\n");
        return 1;
    }

	shared_ptr<IModel> model = make_shared<DartModel>();
	for (int i = 1; i < argc; ++i)
	{
		string value = (argv[i]);
		cout << value << endl << endl;
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
		else
		{
			// Anything parameter without -I prefix
			if (!model->setSourceDirectory(value))
			{
				view->showMessage("WARNING! Incorrect PATH \"" + value + "\" is entered!");
			}
		}
	}

	if (model->getSourseDirectoryName().empty())
	{
		view->showMessage("Error! Source's directory not exist on filesystem.\n");
		return 2;
	}
    auto presenter = make_unique<Presenter>(view, model);
    presenter->startAnalyze();

    return 0;
}
