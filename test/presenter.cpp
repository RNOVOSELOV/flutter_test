#include "presenter.h"
#include "consoleView.h"

#include <iostream>

Presenter::Presenter(shared_ptr<IView> view, shared_ptr<IModel> model)
	:	view(view),
		model (model)
{
	this->view->setListener(this);
}

Presenter::~Presenter()
{
	view->eraseListener();
	cout << " ~ Presenter" << endl;
}

void Presenter::startAnalyze()
{
	this->view->showWelcome(this->model->getSourseDirectory());
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
