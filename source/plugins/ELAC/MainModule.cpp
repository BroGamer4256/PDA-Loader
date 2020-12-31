#include "MainModule.h"
#include <filesystem>

namespace ELAC
{
	typedef std::filesystem::path fspath;

	std::wstring *MainModule::moduleDirectory;

	const wchar_t* MainModule::DivaWindowName = L"Hatsune Miku Project DIVA Arcade";
	const wchar_t* MainModule::GlutDefaultName = L"GLUT";

	HWND MainModule::DivaWindowHandle;
	HMODULE MainModule::Module;

	std::wstring MainModule::GetModuleDirectory()
	{
		if (moduleDirectory == nullptr)
		{
			WCHAR modulePathBuffer[MAX_PATH];
			GetModuleFileNameW(MainModule::Module, modulePathBuffer, MAX_PATH);

			fspath configPath = fspath(modulePathBuffer).parent_path();
			moduleDirectory = new std::wstring(configPath.wstring());
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