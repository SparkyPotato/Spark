@echo off
pushd ..\

echo Generating project files for Visual Studio...

set vswhere="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
call %vswhere% -latest -property catalog_productLineVersion>tmp
set /p vsVersion=<tmp
del /q /f tmp

if %vsVersion%==2019 call Dependencies\Premake\premake5.exe vs2019
if %vsVersion%==2017 call Dependencies\Premake\premake5 vs2017

popd
pause
