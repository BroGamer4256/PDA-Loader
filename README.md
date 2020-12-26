# PDA Loader

**PDA Loader** lets you load plugins into **Project Diva Arcade** by placing them into a **"plugins"** folder, it includes useful ones such as input emulation.

## License and Legal

The code included in this repository is intended for the solely usage of investigative reasons for the **PDAFT** videogame.
Based on your current jurisdiction, it may be illegal to tamper with an arcade cabinet or its hardware and so it would be the usage of this software.
**You must have your own legal dump of the game to use this software.**
Additionally, a MIT License is provided.

## Usage

**IMPORTANT:** Do *not* edit any of the *.ini* configuration files with Notepad. Use [Notepad++](https://notepad-plus-plus.org/download) (or WordPad) with UTF-8 encoding instead, otherwise your game may crash.

Place any **.dva** plugin you want to use inside the **"plugins"** folder.

Launch **"diva.exe"**.

## Download

[Appveyor artifacts](https://ci.appveyor.com/project/BroGamer4256/pda-loader/build/artifacts)

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

MIT (see "LICENSE").

## FAQ

Always use the original game executable.

Special thanks to **nastys**, **Brolijah**, **Skyth**, **korenkonder**, **Samyuu**, **somewhatlurker**, **SwigS-27**, **crash5band**, **akasaka**, **lybxlpsv** and everyone else who helped at the [Project DIVA Modding 2nd Community Discord](https://discord.gg/cvBVGDZ), come join us if you want to help us to improve this project.

## Developer FAQ

To enable logs and dumps create a folder called "logs" in the root folder, each dump size is 200MB so watch your disk space.

This is a stripdown of [**ThirteenAG's** Ultimate ASI Loader v4.47](https://github.com/ThirteenAG/Ultimate-ASI-Loader/) for **"dnsapi.dll"** usage only.

Solution for **Visual Studio 2019**.
Requires platform toolset **v142**.
Requires **Windows 10 SDK (10.0.17763.0)**.

Requires **Desktop development with C++**.
Requires **Windows Universal CRT SDK**.

Requires [**Microsoft Research Detours Package**](https://github.com/microsoft/Detours) (included in the solution).
Requires [**FreeGLUT 3.0.0 PD**](https://github.com/Rayduxz/FreeGLUT) (included in the solution).
