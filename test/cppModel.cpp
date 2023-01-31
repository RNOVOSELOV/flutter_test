#include "cppModel.h"
#include <iostream>
#include <regex>

CppModel::CppModel()
	: languageSpecificIncludeSubline ("\\#include")
{
	addValidExtension(".h");
	addValidExtension(".hpp");
	addValidExtension(".cpp");
}

CppModel::~CppModel()
{
	cout << "~CppModel" << endl;
}

pair <LineRegExpStatus, string> CppModel::validationAndParcingHeaderLine(string line)
{
	regex directiveRegEx{ "\\s*\\#[\\w\\W]*" };
	regex includeRegEx{ "\\s*" + languageSpecificIncludeSubline + "\\s*<[a-zA-Z0-9]+[.]?[h]?>\\s*" };
	regex localRegEx{ "\\s*" + languageSpecificIncludeSubline + "\\s*\"[a-zA-Z0-9]+[.]?[h]?\"\\s*" };
	regex emptyString{ "^\\s*$" };
	regex commentedLineRegEx{ "\\s*\\/\\/\\s*[\\w\\W]*" };

	if (regex_match(line, includeRegEx))
	{
		return pair(LineRegExpStatus::validIncludeHeader, getHeaderFileName(line, '<', '>'));
	}
	else if (regex_match(line, localRegEx))
	{
		return pair(LineRegExpStatus::validLocalHeader, getHeaderFileName(line, '\"', '\"'));
	}
	else if (regex_match(line, emptyString) || regex_match(line, directiveRegEx) || regex_match(line, commentedLineRegEx))
	{
		return pair(LineRegExpStatus::skipHeaderLine, "");
	}
	return pair(LineRegExpStatus::invalidHeader, "");
}