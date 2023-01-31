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

private:
    string languageSpecificIncludeSubline;

    string getHeaderFileName(const string rawHeader, const char firstSymbol, const char secondSymbol);	// Парсинг строки инклюда, получение имени файла
};

