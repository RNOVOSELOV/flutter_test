#pragma once

#include "modelFactory.h"
#include "dartModel.h"

class DartModelFactory:  public ModelFactory
{
public:
	virtual ~DartModelFactory() {};
	virtual std::shared_ptr <IModel> createModel()
	{
		return std::make_shared<DartModel>();
	}
};