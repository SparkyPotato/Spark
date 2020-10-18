:: Copyright 2020 SparkyPotato

@echo off
pushd ..\

set vswhere="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
call %vswhere% -latest -property catalog_productLineVersion>tmp
set /p vsVersion=<tmp
del /q /f tmp

echo Generating project files for Visual Studio %vsVersion%...

if %vsVersion%==2019 (
	call Dependencies\Premake\premake5.exe vs2019
) else (
	if %vsVersion%==2017 (
		call Dependencies\Premake\premake5.exe vs2017
	) else (
		echo Only Visual Studio 2017 and 2019 are supported!
	)
)

popd
pause
