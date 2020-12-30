md data\1.01\plugins
copy Release\dinput8.dll data\1.01
copy Release\MLAC.dva data\1.01\plugins
copy Release\Patches.dva data\1.01\plugins
copy Release\Launcher.dva data\1.01\plugins
copy Config\1.01\components.ini data\1.01\plugins
copy Config\1.01\keyconfig.ini data\1.01\plugins
copy Config\1.01\playerdata.ini data\1.01\plugins
copy Config\1.01\patches.ini data\1.01\plugins

md data\6.00\plugins
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
7z a -t7z ..\6.00.7z 6.00
cd ..