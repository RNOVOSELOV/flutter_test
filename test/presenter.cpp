#include "presenter.h"
#include "consoleView.h"

#include <iostream>

Presenter::Presenter(std::shared_ptr<IView> view, std::shared_ptr<IModel> model)
	:	view(view),
		model (model)
{
	this->view->setListener(this);
}

Presenter::~Presenter()
{
	view->eraseListener();
}

void Presenter::startAnalyze()
{
	this->view->showWelcome(this->model->getSourseDirectoryName());
}

void Presenter::onWelcomeKeyPressed(char value)
{
	if (value == 'Y' || value == 'y')
	{
		model->startExplore();
		view->showSourceTreeStructure(model->getTreeNodes());
		view->showIncludeFrequencies(model->getIncludeFilesFreq());
	}
}
