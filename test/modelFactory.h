#pragma once

#include <iostream>
#include "imodel.h"

using namespace std;

class ModelFactory {
public:
	virtual ~ModelFactory() = default;
	virtual shared_ptr <IModel> createModel() = 0;
};