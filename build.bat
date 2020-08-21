@echo off

set includeDir=..\include\
set defaultCompilerFlags=/I %includeDir% /nologo /Zi /FC
set executable=TheDarkAdventure.exe

if not exist build mkdir build

pushd build

cl %defaultCompilerFlags% ..\src\win32_platform.c /link /out:%executable% user32.lib gdi32.lib

popd