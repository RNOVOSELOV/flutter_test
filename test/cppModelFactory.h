#pragma once

#include "modelFactory.h"
#include "cppModel.h"

class CppModelFactory : public ModelFactory
{
public:
	virtual ~CppModelFactory() {};
	virtual std::shared_ptr <IModel> createModel()
	{
		return std::make_shared<CppModel>();
	}
};