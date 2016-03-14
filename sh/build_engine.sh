#!/bin/bash

cd ..

#Clone repos
git clone https://github.com/bulletphysics/bullet3.git
git clone https://github.com/g-truc/glm.git
git clone https://github.com/assimp/assimp.git

mkdir src/lib
mkdir src/lib/dynamic
mkdir src/lib/static

#Make assimp shared libs
cd assimp
cmake CMakeLists.txt -G 'Unix Makefiles'
make

#Move shared libs to engine lib folder
cd lib
find ./ -type f -iname "*.so*" -print0 | xargs -0 -Ilibfiles mv libfiles ../../src/lib/dynamic/


#Move header files to include dir
cd ../include
mv assimp ../../src/headers/assimp_includes
cd ../..


#Move header files to include dir
cd glm
find ./ -type f -iname "*.txt" -print0 | xargs -0 -Isrcfiles rm srcfiles
mv glm ../src/headers/glm_includes

#Make bullet shared libs
cd ../bullet3
mkdir bullet-build
cd bullet-build
cmake .. -G "Unix Makefiles" -DINSTALL_LIBS=ON -DBUILD_SHARED_LIBS=ON
make -j4

#Move shared libs to engine lib folder
find ./ -type f -iname "*.so*" -print0 | xargs -0 -Ilibfiles mv libfiles ../../src/lib/dynamic/

#Remove unused files
cd ../src
find ./ -type f -iname "*.cpp" -print0 | xargs -0 -Isrcfiles rm srcfiles
find ./ -type f -iname "*.txt" -print0 | xargs -0 -Isrcfiles rm srcfiles
find ./ -type f -iname "*.lua" -print0 | xargs -0 -Isrcfiles rm srcfiles
cd ..

#Move header files to include dir
mv src ../src/headers/bullet3_includes
cd ..

#Delete repos
rm -rf bullet3
rm -rf glm
rm -rf assimp

#Build engine
make clean
make