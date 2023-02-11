#pragma once

#include "iview.h"
#include "model.h"

class Presenter:IViewEvents
{
public:
	Presenter(std::shared_ptr<IView>, std::shared_ptr<IModel>);
	virtual ~Presenter();
	virtual void onWelcomeKeyPressed(char value);
	void startAnalyze();
private:
	std::shared_ptr<IView> view;
	std::shared_ptr<IModel> model;
};

