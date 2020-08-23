@echo off

set includeDir=..\include\
set defaultCompilerFlags=/I %includeDir% /nologo /Zi /FC /O2 /MP
set warnings=/W4 /WX /wd4201 /wd4100
set defines=
REM /DTESTING

set executable=ADarkAdventure.exe

if not exist build mkdir build

pushd build

cl %defaultCompilerFlags% %warnings% %defines% ..\src\win32_platform.c /link /out:%executable% user32.lib gdi32.lib

popd