:: Copyright 2020 SparkyPotato

@echo off

:: Ensure we're in the batch file's directory
pushd %~dp0

:: Go to the engine root
cd ..\..\..

:: If the release version of SparkBuild exists, call it
if exist "Engine\Binaries\Release\SparkBuild\SparkBuild.exe" (
	call "Engine\Binaries\Release\SparkBuild\SparkBuild.exe" %*
) else (
	:: Get the location of Visual Studio and MSBuild
	set vswhere="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
	call %vswhere% -latest -property installationPath>tmp
	set /p vsPath=<tmp
	del /q /f tmp
	set msbuild="%vsPath%\MSBuild\Current\Bin\MSBuild.exe"
	:: Build SparkBuild with MSBuild, and call it
	call %msbuild% Intermediate\SparkBuild.vcxproj /p:Configuration=Release /p:Platform=x64
	cls
	call "Engine\Binaries\Release\SparkBuild\SparkBuild.exe" %*
)

popd
