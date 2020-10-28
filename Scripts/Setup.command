# Copyright 2020 SparkyPotato

cd ..
startDir=$(pwd)

mkdir -p Dependencies
cd Dependencies
echo "Installing Dependencies"

echo "Installing Premake"
mkdir Premake
cd Premake
echo "  Downloading"
curl -L -o premake.zip https://github.com/premake/premake-core/archive/master.zip

echo "  Extracting"
unzip premake.zip
rm -f premake.zip

cd premake-core-master
echo "  Building"
make -f Bootstrap.mak osx

echo "  Copying Binaries"
cd bin/release
mv premake5 ../../../
cd ../..
mv LICENSE.txt ../
cd ..
rm -rf premake-core-master
chmod 755 premake5

cd $startDir
