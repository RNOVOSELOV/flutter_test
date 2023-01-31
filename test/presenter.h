#pragma once

#include "iview.h"
#include "model.h"

class Presenter:IViewEvents
{
public:
	Presenter(shared_ptr<IView>, shared_ptr<IModel>);
	virtual ~Presenter();
	virtual void onWelcomeKeyPressed(char value);
	void startAnalyze();
private:
	shared_ptr<IView> view;
	shared_ptr<IModel> model;
};

