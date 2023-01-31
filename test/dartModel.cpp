#include "dartModel.h"

#include <iostream>
#include <regex>

DartModel::DartModel()
	: languageSpecificIncludeSubline("package:gift_manager")
{
	addValidExtension(".dart");
}

DartModel::~DartModel()
{
	cout << "~DartModel" << endl;
}

pair <LineRegExpStatus, string> DartModel::validationAndParcingHeaderLine(string line)
{
	regex includeRegEx{ "\\s*import\\s*\\'package:[a-zA-Z0-9_\\/]+.dart\\'\\s*[;]+\\s*" };
	regex localRegEx{ "\\s*import\\s*\\'" + languageSpecificIncludeSubline + "\\/[a-zA-Z0-9_\\/]+.dart\\'\\s*[;]+\\s*" };
	regex emptyString{ "^\\s*$" };
	regex commentedLineRegEx{ "\\s*\\/\\/\\s*[\\w\\W]*" };

	cout << line << endl;
	if (regex_match(line, localRegEx))
	{
		cout << " localRegEx " << endl;
		return pair(LineRegExpStatus::validLocalHeader, getHeaderFileName(line, '\'', '\''));
	}
	else if (regex_match(line, includeRegEx))
	{
		cout << " includeRegEx " << endl;
		return pair(LineRegExpStatus::validIncludeHeader, getHeaderFileName(line, '\'', '\''));
	}
	else if (regex_match(line, emptyString) || regex_match(line, commentedLineRegEx))
	{
		return pair(LineRegExpStatus::skipHeaderLine, "");
	}
	return pair(LineRegExpStatus::invalidHeader, "");
}
