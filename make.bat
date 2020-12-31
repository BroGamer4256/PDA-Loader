msbuild -p:Configuration=Release;Platform=x86
cd source\plugins\MLAC
msbuild -p:Configuration=101
msbuild -p:Configuration=130
msbuild -p:Configuration=301
cd ..\..\..\

publish