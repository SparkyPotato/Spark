:: Copyright 2020 SparkyPotato

@echo off
pushd %~dp0
cd ..\..\..

set startDir=%cd%

set vswhere="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
call %vswhere% -latest -property installationPath>tmp
set /p vsPath=<tmp
del /q /f tmp

@call "%vsPath%\VC\Auxiliary\Build\vcvars64.bat">Intermediate\Cache\tmp

%startDir%\Binaries\SparkBuild\Release\SparkBuild.exe %*
echo.

tup --quiet

popd
