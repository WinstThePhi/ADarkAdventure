@echo off

set executable=ADarkAdventure.exe
set game=ADarkAdventure.dll

set includes=/I ..\include\ /I ..\engine\

set defaultCompilerFlags=%includes% /nologo /O2 /Zi /FC /MP

set platformLinkSettings=/link /out:%executable% user32.lib gdi32.lib /incremental:no

set gameLinkSettings=/link /DLL /EXPORT:Game_UpdateAndRender /EXPORT:Game_Start /out:%game% 

set warnings=/W4 /WX /wd4201 /wd4100 /wd4189

set defines=/DFPS_CAP

set platformCode=..\engine\ADarkEngine\win32\ADarkEngine_win32_platform.c
set gameCode=..\src\game.c

if not exist build mkdir build

pushd build

if NOT "%1"=="reload" (
	cl %defines% %defaultCompilerFlags% %warnings% %platformCode% %platformLinkSettings%
)

cl %defines% %defaultCompilerFlags% %warnings% %gameCode% %gameLinkSettings%

popd