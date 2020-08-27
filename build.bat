@echo off

set executable=ADarkAdventure.exe
set game=ADarkAdventure.dll

set includes=/I ..\include\ /I ..\engine\

set defaultCompilerFlags=%includes% /nologo /DEBUG /FC /MP

set platformLinkSettings=/link /out:%executable% user32.lib gdi32.lib /incremental:no /DEBUG

set exports=/EXPORT:Game_UpdateAndRender /EXPORT:Game_Start /EXPORT:Game_End
set gameLinkSettings=/link /DLL %exports% /out:%game%

set warnings=/W4 /WX /wd4201 /wd4100 /wd4189 /wd4996

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