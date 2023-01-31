#pragma once
#include "model.h"

class CppModel :
    public Model
{
public:
    CppModel();
    virtual ~CppModel();

protected:
    virtual pair <LineRegExpStatus, string> validationAndParcingHeaderLine(string line);
};

