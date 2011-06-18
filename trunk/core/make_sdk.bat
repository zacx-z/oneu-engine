@echo off
echo "生成SDK文件，请确保release版已经编译。"
pause
for %%d in ("OUE", "Base") do (
	mkdir "..\OneU SDK\include\"%%d
	for %%f in (%%d/"*.h") do (
		copy %%d"\%%f" "..\OneU SDK\include\"%%d\%%f
	)
)
mkdir "..\OneU SDK\lib\x86"
copy "release\OUE.so" "..\OneU SDK\lib\x86\OUE.so"
copy "release\OUE.lib" "..\OneU SDK\lib\x86\OUE.lib"
copy "release\_lua.dll" "..\OneU SDK\lib\x86\_lua.dll"
copy "release\_lua.lib" "..\OneU SDK\lib\x86\_lua.lib"
copy "release\Base.dll" "..\OneU SDK\lib\x86\Base.dll"
copy "release\Base.lib" "..\OneU SDK\lib\x86\Base.lib"
copy "lib\msvcrt-ruby190.dll" "..\OneU SDK\lib\x86\msvcrt-ruby190.dll"
copy "lib\msvcrt-ruby190.lib" "..\OneU SDK\lib\x86\msvcrt-ruby190.lib"

mkdir "..\OneU SDK\bin\script"
copy "release\OUE.so"  "..\OneU SDK\bin\OUE.so"
copy "release\_lua.dll" "..\OneU SDK\bin\_lua.dll"
copy "release\Base.dll" "..\OneU SDK\bin\Base.dll"
copy "lib\msvcrt-ruby190.dll" "..\OneU SDK\bin\msvcrt-ruby190.dll"

copy "release\Game.exe" "..\OneU SDK\bin\Game.exe"

rem write default main.lua
copy "default_script\main.rb" "..\OneU SDK\bin\script\main.rb"

copy "Doxygen\Document\html\OneU.chm" "..\OneU SDK\OneU.chm"

rem copy atoms
mkdir "..\OneU SDK\bin\atom"

pause
