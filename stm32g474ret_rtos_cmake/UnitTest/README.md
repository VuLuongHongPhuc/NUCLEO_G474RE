## Folder
my_project/
├── CMakeLists.txt
└── tests/
    ├──include/
    |   └── test.h
    └── test.cpp

## MSYS2 - ucrt
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-ninja


## Build
mkdir build
cd build

cmake -G Ninja ..
cmake --build .


## Launch test
ctest --output-on-failure

