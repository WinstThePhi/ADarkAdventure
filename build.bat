@echo off

set executable=ADarkAdventure.exe
set game=ADarkAdventure.dll

set includes=/I ..\include\ /I ..\engine\

set defaultCompilerFlags=%includes% /nologo /O2 /Zi /FC /MP

set platformLinkSettings=/link /out:%executable% user32.lib gdi32.lib /incremental:no

set gameLinkSettings=/link /DLL /EXPORT:Game_UpdateAndRender /out:%game% 

set warnings=/W4 /WX /wd4201 /wd4100

REM set defines=
REM /DTESTING

set platformCode=..\src\win32_platform.c
set gameCode=..\src\game.c

if not exist build mkdir build

pushd build

if NOT "%1"=="reload" (
	cl %defaultCompilerFlags% %warnings% %platformCode% %platformLinkSettings%
)

cl %defaultCompilerFlags% %warnings% %gameCode% %gameLinkSettings%

popd