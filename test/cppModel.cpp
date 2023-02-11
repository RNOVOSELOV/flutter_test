#include "cppModel.h"
#include <iostream>
#include <regex>

using std::regex;

CppModel::CppModel()
	: languageSpecificIncludeSubline ("\\#include")
{
	addValidExtension(".h");
	addValidExtension(".hpp");
	addValidExtension(".cpp");
}

CppModel::~CppModel()
{

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

string CppModel::getHeaderFileName(const string rawHeader, const char firstSymbol, const char secondSymbol)
{
	auto it1 = rawHeader.find_first_of(firstSymbol) + 1;
	auto it2 = rawHeader.find_first_of(secondSymbol, it1);
	string str2(it2 - it1, '\0');
	copy(rawHeader.begin() + it1, rawHeader.begin() + it2, str2.begin());
	return str2;
}