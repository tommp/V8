#!/bin/bash

#Clone repos
git clone https://github.com/libRocket/libRocket.git
git clone https://github.com/bulletphysics/bullet3.git

#Make Rocket shared libs
cd libRocket
cd Build
cmake CMakeLists.txt
make

#Move shared libs to engine lib folder
find ./ -type f -name "*.so*" -print0 | xargs -0 -Ilibfiles mv -i libfiles ../../src/lib/dynamic/
cd ..

#Move header files to include dir
cd include/
mv Rocket ../../src/headers/
cd ../../


#Make bullet shared libs
cd bullet3
mkdir bullet-build
cd bullet-build
cmake .. -G "Unix Makefiles" -DINSTALL_LIBS=ON -DBUILD_SHARED_LIBS=ON
make -j4

#Move shared libs to engine lib folder
find ./ -type f -name "*.so*" -print0 | xargs -0 -Ilibfiles mv -i libfiles ../../src/lib/dynamic/
cd ../src/

#Remove .cpp files
find ./ -type f -name "*.cpp" -print0 | xargs -0 -Isrcfiles rm srcfiles
cd ..

#Move header files to include dir
mv src ../src/headers/

#Delete repos
rm -rf libRocket
rm -rf bullet3

#Build engine
cd src
make clean
make