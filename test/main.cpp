#include <iostream>
// #include "hello.h"
#include "consoleView.h"
#include "cppmodel.h"
#include "presenter.h"

int main(int argc, char * argv[])
{
	shared_ptr<IView> view = make_shared <ConsoleView>();
    if (argc <= 1)
    {
		view->showMessage("Error! Number of arguments error. Source direstory isn't setted.\n");
        return 1;
    }

	shared_ptr<IModel> model = make_shared<CppModel>();
	for (int i = 1; i < argc; ++i)
	{
		string value = *(argv + i);
		if (value == "-I" && i != argc - 1)
		{
			string directory = *(argv + ++i);
			auto isValid{ model->setSourceFilesDirectory(directory) };
			if (!isValid)
			{
				view->showMessage("WARNING! Directory \"" + directory + "\" doesn't exist.\n");
			}
		}
		else if (value == "-F" && i != argc - 1)
		{
			string filePath = *(argv + ++i);
			const path directory{ filePath };
			if (exists(directory) && is_regular_file(directory))
			{
				model->setSourceDirectory(canonical(directory));
			}
		}
		else
		{
			// Anything parameter without -I or -F parameter prefix
			model->setSourceDirectory(value);
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
