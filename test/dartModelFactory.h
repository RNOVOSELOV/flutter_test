#pragma once

#include "modelFactory.h"
#include "dartModel.h"

class DartModelFactory:  public ModelFactory
{
public:
	virtual ~DartModelFactory() {};
	virtual shared_ptr <IModel> createModel()
	{
		return make_shared<DartModel>();
	}
};