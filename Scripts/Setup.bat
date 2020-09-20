@echo off
cd ..

set startDir=%cd%

mkdir Dependencies
cd Dependencies
echo Installing Dependencies...

echo Installing Premake...
mkdir Premake
cd Premake
echo 	Downloading...
powershell -Command "Invoke-WebRequest https://github.com/premake/premake-core/archive/master.zip -OutFile premake.zip"

echo 	Extracting...
powershell -Command "Expand-Archive -Force premake.zip ./"
del premake.zip /q /f

cd premake-core-master
echo 	Building...

set vswhere="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
call %vswhere% -latest -property installationPath>tmp
set /p vsPath=<tmp
del /q /f tmp

call "%vsPath%\Common7\Tools\VsDevCmd.bat"
nmake -f Bootstrap.mak windows
cd %startDir%

cd Dependencies\Premake\premake-core-master
echo 	Copying Binaries...
cd bin
cd release
xcopy "premake5.exe" "..\..\..\"
cd ..\..\
xcopy "LICENSE.txt" "..\"
cd ..\
rmdir /q /s premake-core-master
cd %startDir%

cd Scripts
call GenerateProjectFiles.bat

pause
