:: Copyright 2020 SparkyPotato

@echo off
pushd %~dp0
cd ..\..\..

set startDir=%cd%
%startDir%\Binaries\SparkBuild\SparkBuild.exe source="%1" %2

cd %1

echo.

tup upd

popd
