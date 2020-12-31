#include "TextFile.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace MLAC::FileSystem
{
	TextFile::TextFile(const std::wstring &path)
	{
		FileName = path;
	}

	TextFile::TextFile(const std::wstring &directory, const std::wstring &file)
	{
		std::wstring fullPath = directory + L"/" + file;
		FileName = fullPath;
	}

	TextFile::~TextFile()
	{
	}

	bool TextFile::OpenRead()
	{
		fs::path configPath(FileName);

		if (!fs::exists(configPath))
			return false;

		std::ifstream fileStream(FileName);
		
		if (!fileStream.is_open())
			return false;

		Parse(fileStream);

		fileStream.close();

		return true;
	}
}
