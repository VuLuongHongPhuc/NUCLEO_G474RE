## Folder
<pre>
my_project/
├── CMakeLists.txt
└── tests/
    ├──include/
    |   └── test.h
    └── test.cpp
</pre>

## Build environnement
### On windows
- Install packages for MSYS2 ucrt
```text
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-ninja
pacman -S mingw-w64-ucrt-x86_64-gtest
```
- Install vscode extension CMake Tools
- On PROJECT STATUS, select Configuration > GCC 15.2.0 x86_64-w64-mingw32 (ucrt)

### On wsl ubuntu
- Install packages dev
```text
sudo apt update
sudo apt install build-essential cmake ninja-build gdb
```
- Install google test lib
```text
sudo apt install libgtest-dev libgmock-dev
```
- compile/build google test if needed
```text
cd /usr/src/gtest
sudo cmake .
sudo make
sudo cp lib/*.a /usr/lib/
```

## Build
```text
mkdir build
cd build
cmake -G Ninja ..
cmake --build .
```
or
```text
mkdir build
cmake -S . -B build -G Ninja
cmake --build build
```

## Launch test
```text
ctest --output-on-failure
```

