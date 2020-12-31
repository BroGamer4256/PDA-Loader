#include "MainModule.h"
#include <filesystem>

namespace OLAC
{
	typedef std::filesystem::path fspath;

	std::string *MainModule::moduleDirectory;

	const wchar_t* MainModule::DivaWindowName = L"Project DIVA Arcade";
	const wchar_t* MainModule::GlutDefaultName = L"GLUT";

	HWND MainModule::DivaWindowHandle;
	HMODULE MainModule::Module;

	std::string MainModule::GetModuleDirectory()
	{
		if (moduleDirectory == nullptr)
		{
			CHAR modulePathBuffer[MAX_PATH];
			GetModuleFileNameA(MainModule::Module, modulePathBuffer, MAX_PATH);

			fspath configPath = fspath(modulePathBuffer).parent_path();
			moduleDirectory = new std::string(configPath.string());
		}

		return *moduleDirectory;
	}

	RECT MainModule::GetWindowBounds()
	{
		RECT windowRect;
		GetWindowRect(DivaWindowHandle, &windowRect);

		return windowRect;
	}
}