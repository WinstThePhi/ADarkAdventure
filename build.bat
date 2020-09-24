@echo off

if "%1"=="clean" (
	REM del /f /s /q build\*
	exit
)

set executable=ADarkAdventure.exe
set game=game.dll

set includes=/I ..\include\ /I ..\engine\

set defaultCompilerFlags=%includes% /O2 /nologo /Zi /FC /MP 

set platformLinkSettings=/link user32.lib gdi32.lib opengl32.lib /incremental:no /RELEASE

set exports=/EXPORT:Game_UpdateAndRender /EXPORT:Game_Start /EXPORT:Game_End
set gameLinkSettings=/link /DLL %exports% /out:%game% /RELEASE

set warnings=/W4 /WX /wd4201 /wd4100 /wd4189 /wd4996
 
set defines=/DFPS_CAP /DSTRETCH /DHIDE_CURSOR /DQUEUE_TEST
 
set platformCode=..\engine\ADarkEngine\win32\ADarkEngine_win32_platform.c
set gameCode=..\src\game.c

if not exist build mkdir build

if "%1"=="generator" (
	pushd build
	
	start /b /wait ..\engine\ADarkEngine\code_generator\build.bat

	REM delete unnecessary files
	del /q /f /s *.obj *.lib *.ilk *.exp

	REM copy to code_generator
	move code_generator.exe code_generator\code_generator.exe
	move simple_preprocessor.exe code_generator\simple_preprocessor.exe

	popd
	exit
)

pushd build

REM start /b /wait ..\engine\ADarkEngine\code_generator\build.bat

pushd ..\data
..\build\code_generator\code_generator.exe game_state.dark
popd

if NOT "%1"=="reload" (
	start /b /wait cl %defines% %defaultCompilerFlags% %warnings% %platformCode% %platformLinkSettings%
)

start /b /wait cl %defines% %defaultCompilerFlags% %warnings% %gameCode% %gameLinkSettings%

REM delete unnecessary files
del /q /f /s *.obj *.lib *.ilk *.exp

popd

