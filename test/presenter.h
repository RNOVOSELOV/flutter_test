#pragma once

#include "iview.h"
#include "model.h"

class Presenter:IViewEvents
{
public:
	Presenter();
	~Presenter();
	bool initModel(int argc, char* argv[]);
	virtual void onWelcomeKeyPressed(char value);

	void startAnalyze();
private:
	unique_ptr<IView> view;
	Model* model;
};

