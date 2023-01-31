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

void IModel::setSourceDirectory(string filePath)
{
	const path directory{ filePath };
	if (exists(directory))
	{
		setSourceDirectory(canonical(directory));
	}
}

void IModel::setSourceDirectory(const path& directory)
{
	sourcePath = directory;
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