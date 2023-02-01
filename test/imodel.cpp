#include <filesystem>
#include "imodel.h"

IModel::IModel()
{
	sourcePath.clear();
}

bool IModel::setSourceFilesDirectory(string directory)
{
	const path sourcesDirectory{ directory };
	if (isValidPath(sourcesDirectory))
	{
		includeDirs.push_back(canonical(sourcesDirectory));
		return true;
	}
	return false;
}

bool IModel::setSourceDirectory(string filePath)
{
	const path directory{ filePath };
	if (exists(directory) && (is_regular_file(directory) || is_directory(directory)))
	{
		return setSourceDirectory(canonical(directory));
	}
	return false;
}

bool IModel::setSourceDirectory(const path& directory)
{
	sourcePath = directory;
	return true;
}

bool IModel::setProjectName(string value)
{
	const path directory{ value };
	if (exists(directory) && is_directory(directory))
	{
		projectName = directory;
		return true;
	}
	return false;
}

bool IModel::isValidPath(const path& p, bool checkIsFile)
{
	if (!exists(p))
	{
		return false;
	}
	if (checkIsFile && !is_regular_file(p))
	{
		return false;
	}
	return true;
}