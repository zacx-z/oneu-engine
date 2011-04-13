echo "生成SDK文件，请确保release版已经编译。"
pause
@echo off
for %%d in ("OUE", "Base") do (
	mkdir "..\OneU SDK\include\"%%d
	for %%f in (%%d/"*.h") do (
		copy %%d"\%%f" "..\OneU SDK\include\"%%d\%%f
	)
)
mkdir "..\OneU SDK\lib\x86"
copy "release\OUE.dll" "..\OneU SDK\lib\x86\OUE.dll"
copy "release\OUE.lib" "..\OneU SDK\lib\x86\OUE.lib"
copy "release\Base.dll" "..\OneU SDK\lib\x86\Base.dll"
copy "release\Base.lib" "..\OneU SDK\lib\x86\Base.lib"

mkdir "..\OneU SDK\bin\script"
copy "release\OUE.dll"  "..\OneU SDK\bin\OUE.dll"
copy "release\Base.dll" "..\OneU SDK\bin\Base.dll"

copy "release\Game.exe" "..\OneU SDK\bin\Game.exe"

echo "a" | xcopy "Game\script\lua\lib" "..\OneU SDK\bin\script\lua\lib" /E /I

rem write default main.lua
copy "default_main.lua" "..\OneU SDK\bin\script\lua\main.lua"

copy "Doxygen\Document\html\OneU.chm" "..\OneU SDK\OneU.chm"

rem copy atoms
mkdir "..\OneU SDK\bin\atom"
copy "release\AtomDemo.at" "..\OneU SDK\bin\atom\Demo.at"

pause
