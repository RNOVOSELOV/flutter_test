#pragma once

#include <iostream>
#include "imodel.h"

class ModelFactory {
public:
	virtual ~ModelFactory() = default;
	virtual std::shared_ptr <IModel> createModel() = 0;
};