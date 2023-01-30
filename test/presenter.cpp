#include "presenter.h"
#include "consoleView.h"

Presenter::Presenter()
{
	view = new ConsoleView();
	model = new Model();
	this->view->setListener(this);
}

Presenter::~Presenter()
{
	view->eraseListener();
	delete view;
	delete model;
}

bool Presenter::initModel(int argc, char* argv[])
{
	auto isValidDirectory{ model->setSourseDirectory(*(argv + 1)) };
	if (!isValidDirectory)
	{
		view->showMessage("Error! Source's directory doesn't exist");
		return false;
	}
	for (int i = 2; i < argc; ++i)
	{
		string value = *(argv + i);
		if (value == "-I")
		{
			string directory = *(argv + ++i);
			auto isValid{ model->setSourseFilesDirestory(directory) };
			if (!isValid)
			{
				view->showMessage("WARNING! Directory " + directory +  " doesn't exist.\n");
			}
		}
	}
	return true;
}

void Presenter::startAnalyze()
{
	this->view->showWelcome(this->model->getSourseDirectory());
}

void Presenter::onWelcomeKeyPressed(char value)
{
	if (value == 'Y' || value == 'y')
	{
		switch (model->startExplore())
		{
		case AnalyzeResult::successful:
			view->showSourceTreeStructure(model->getTreeNodes());
			view->showIncludeFrequencies(model->getIncludeFilesFreq());
			break;
		case AnalyzeResult::sourceDirError:
			view->showMessage("Source's directory doesn't exist");
			break;
		}
	}
}
