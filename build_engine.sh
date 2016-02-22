#!/bin/bash

#Clone repos
git clone https://github.com/libRocket/libRocket.git
git clone https://github.com/bulletphysics/bullet3.git
git clone https://github.com/g-truc/glm.git

mkdir src/lib

#Move header files to include dir
cd glm
find ./ -type f -iname "*.txt" -print0 | xargs -0 -Isrcfiles rm srcfiles
mv glm ../src/headers/glm_includes

#Make Rocket shared libs
cd ../libRocket
cd Build
cmake CMakeLists.txt
make

#Move shared libs to engine lib folder
find ./ -type f -iname "*.so*" -print0 | xargs -0 -Ilibfiles mv libfiles ../../src/lib/
cd ../Include

#Move header files to include dir
mv Rocket ../../src/headers/rocket_includes
cd ../..


#Make bullet shared libs
cd bullet3
mkdir bullet-build
cd bullet-build
cmake .. -G "Unix Makefiles" -DINSTALL_LIBS=ON -DBUILD_SHARED_LIBS=ON
make -j4

#Move shared libs to engine lib folder
find ./ -type f -iname "*.so*" -print0 | xargs -0 -Ilibfiles mv libfiles ../../src/lib/

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
rm -rf libRocket
rm -rf bullet3
rm -rf glm

#Build engine
cd src
make clean
make