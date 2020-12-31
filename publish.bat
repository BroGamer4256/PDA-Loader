md data\1.01\plugins
md data\1.01\patches
copy Release\dinput8.dll data\1.01
move Source\Plugins\MLAC\101\MLAC_101.dva data\1.01\plugins\MLAC.dva
copy Release\Patches.dva data\1.01\plugins
copy Release\Launcher.dva data\1.01\plugins
copy Config\1.01\components.ini data\1.01\plugins
copy Config\1.01\keyconfig.ini data\1.01\plugins
copy Config\1.01\playerdata.ini data\1.01\plugins
copy Config\1.01\patches.ini data\1.01\plugins

md data\1.30\plugins
md data\1.30\patches
copy Release\dinput8.dll data\1.30
move Source\Plugins\MLAC\130\MLAC_130.dva data\1.30\plugins\MLAC.dva
copy Release\Patches.dva data\1.30\plugins
copy Release\Launcher.dva data\1.30\plugins
copy Config\1.30\components.ini data\1.30\plugins
copy Config\1.30\keyconfig.ini data\1.30\plugins
copy Config\1.30\playerdata.ini data\1.30\plugins
copy Config\1.30\patches.ini data\1.30\plugins

md data\3.01\plugins
md data\3.01\patches
copy Release\dinput8.dll data\3.01
move Source\Plugins\MLAC\301\MLAC_301.dva data\3.01\plugins\MLAC.dva
copy Release\Patches.dva data\3.01\plugins
copy Release\Launcher.dva data\3.01\plugins
copy Config\3.01\components.ini data\3.01\plugins
copy Config\3.01\keyconfig.ini data\3.01\plugins
copy Config\3.01\playerdata.ini data\3.01\plugins
copy Config\3.01\patches.ini data\3.01\plugins

md data\6.00\plugins
md data\6.00\patches
copy Release\dinput8.dll data\6.00
copy Release\ELAC.dva data\6.00\plugins
copy Release\Patches.dva data\6.00\plugins
copy Release\Launcher.dva data\6.00\plugins
copy Config\6.00\components.ini data\6.00\plugins
copy Config\6.00\graphics.ini data\6.00\plugins
copy Config\6.00\keyconfig.ini data\6.00\plugins
copy Config\6.00\playerdata.ini data\6.00\plugins
copy Config\6.00\patches.ini data\6.00\plugins
copy Config\6.00\pv_modules.csv data\6.00\plugins

cd data
7z a -t7z ..\1.01.7z 1.01
7z a -t7z ..\1.30.7z 1.30
7z a -t7z ..\3.01.7z 3.01
7z a -t7z ..\6.00.7z 6.00
cd ..