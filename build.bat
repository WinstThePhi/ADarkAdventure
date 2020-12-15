@echo off

if "%1"=="clean" (
	REM del /f /s /q build\*
	exit
)

set compiler=cl
REM set compiler=cl

set executable=ADarkAdventure.exe
set game=game.dll

set includes=/I ..\include\ /I ..\engine\ADarkEngine\

set defaultCompilerFlags=%includes% /nologo /Zi /FC /Fm
REM -fdiagnostics-absolute-paths -O2

set platformLinkSettings=/link /incremental:no /STACK:0x100000,0x100000 

set exports=/EXPORT:Game_UpdateAndRender /EXPORT:Game_Start /EXPORT:Game_End
set gameLinkSettings=/link %exports% /DLL /out:%game% /incremental:no /STACK:0x100000,0x100000 

set warnings=/W4 /WX /wd4201 /wd4100 /wd4189 /wd4996 /wd4505 /wd4273
REM -Wno-unused-function -Wno-unused-variable

if %compiler%==clang-cl (
	set warnings=%warnings% -Wno-unused-function -Wno-unused-variable
	set defaultCompilerFlags=%defaultCompilerFlags% -fdiagnostics-absolute-paths -O2
)

set defines=/DCOMPILER_MSVC
 
set platformCode=..\engine\ADarkEngine\win32\win32_platform.cpp
set gameCode=..\src\game.cpp

if not exist build mkdir build

if "%1"=="generator" (
	pushd build
	
	start /b /wait ..\engine\ADarkEngine\code_generator\build.bat

	REM delete unnecessary files
	del /q /f *.lib *.obj *.ilk *.exp

	REM copy to code_generator
	REM move code_generator.* code_generator\
	move simple_preprocessor.* code_generator\

	popd
	exit
)

pushd build

REM start /b /wait ..\engine\ADarkEngine\code_generator\build.bat

pushd ..\data
REM ..\build\code_generator\code_generator.exe game_state.dark
popd

if NOT "%1"=="reload" (
	start /b /wait %compiler% %defines% %defaultCompilerFlags% %warnings% %platformCode% %platformLinkSettings%
)

start /b /wait %compiler% %defines% %defaultCompilerFlags% %warnings% %gameCode% %gameLinkSettings%

REM delete unnecessary files
del /q /f *.obj *.lib *.ilk *.exp

popd

