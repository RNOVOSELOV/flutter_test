#pragma once
#include "model.h"

class DartModel :
    public Model
{
public:
    DartModel();
    virtual ~DartModel();

protected:
    virtual pair <LineRegExpStatus, string> validationAndParcingHeaderLine(string line);

private:
    string languageSpecificIncludeSubline;
};

