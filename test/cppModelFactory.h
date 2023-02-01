#pragma once

#include "modelFactory.h"
#include "cppModel.h"

class CppModelFactory : public ModelFactory
{
public:
	virtual ~CppModelFactory() {};
	virtual shared_ptr <IModel> createModel()
	{
		return make_shared<CppModel>();
	}
};