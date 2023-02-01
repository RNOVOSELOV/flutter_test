#include "dartModel.h"

#include <iostream>
#include <regex>

DartModel::DartModel()
	: languageSpecificIncludeSubline("package:" + getProjectName().filename().u8string())
{
	addValidExtension(".dart");
}

DartModel::~DartModel()
{
}

pair <LineRegExpStatus, string> DartModel::validationAndParcingHeaderLine(string line)
{
	languageSpecificIncludeSubline = "package:" + getProjectName().filename().u8string();
	regex includeRegEx{ "\\s*import\\s*\\'package:[a-zA-Z0-9_\\/]+.dart\\'\\s*[;]+\\s*" };
	regex localRegEx{ "\\s*import\\s*\\'" + languageSpecificIncludeSubline + "\\/[a-zA-Z0-9_\\/]+.dart\\'\\s*[;]+\\s*" };
	regex emptyString{ "^\\s*$" };
	regex commentedLineRegEx{ "\\s*\\/\\/\\s*[\\w\\W]*" };

	if (regex_match(line, localRegEx))
	{
		return pair(LineRegExpStatus::validLocalHeader, getHeaderLocalFileName(line, '/', '\''));
	}
	else if (regex_match(line, includeRegEx))
	{
		return pair(LineRegExpStatus::validIncludeHeader, getHeaderImportFileName(line, '/', '\''));
	}
	else if (regex_match(line, emptyString) || regex_match(line, commentedLineRegEx))
	{
		return pair(LineRegExpStatus::skipHeaderLine, "");
	}
	return pair(LineRegExpStatus::invalidHeader, "");
}

string DartModel::getHeaderImportFileName(const string rawHeader, const char firstSymbol, const char secondSymbol)
{
	auto it1 = rawHeader.find_last_of(firstSymbol) + 1;
	auto it2 = rawHeader.find_first_of(secondSymbol, it1);
	string str2(it2 - it1, '\0');
	copy(rawHeader.begin() + it1, rawHeader.begin() + it2, str2.begin());
	return str2;
}

string DartModel::getHeaderLocalFileName(const string rawHeader, const char firstSymbol, const char secondSymbol)
{
	auto it1 = rawHeader.find_first_of(firstSymbol) + 1;
	auto it2 = rawHeader.find_first_of(secondSymbol, it1);
	string str2(it2 - it1, '\0');
	copy(rawHeader.begin() + it1, rawHeader.begin() + it2, str2.begin());

	std::replace(str2.begin(), str2.end(), '/', (char)std::filesystem::path::preferred_separator); //
	return getProjectName().append("lib").append(str2).u8string();
}