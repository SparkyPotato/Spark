:: Copyright 2020 SparkyPotato

@echo off

:: Ensure we're in the batch file's directory
pushd %~dp0

:: Go to the engine root
cd ..\..\..\..

set vswhere="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
call %vswhere% -latest -property installationPath>tmp
set /p vsPath=<tmp
del /q /f tmp

:: If the release version of SparkBuild exists, call it
if exist "Binaries\Release\SparkBuild\SparkBuild.exe" (
	call "%vsPath%\VC\Auxiliary\Build\vcvars64.bat">Intermediate\tmp
	call "Binaries\Release\SparkBuild\SparkBuild.exe" %*
) else (
	echo Error: SparkBuild needs to be built in the Release configuration!
)

popd
