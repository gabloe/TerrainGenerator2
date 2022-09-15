TerrainGenerator [![Build Status](https://app.travis-ci.com/gabloe/TerrainGenerator.svg?branch=main)](https://app.travis-ci.com/gabloe/TerrainGenerator)
=======================

Built from the [OpenGL CMake Skeleton](https://github.com/ArthurSonzogni/OpenGL_CMake_Skeleton) template

Creates procedurially generated terrain, and supports FPS style movement around the terrain.

![demo](demo.gif)

goals :
1. Use Perlin noise to generate the terrain mesh
   * Must include valleys and peaks
2. Add dynamic lighting with multiple light sources
3. Dynamic terrain expansion based on global position of camera
4. Dynamic textures based on heightmap
   * E.g. maybe tall peaks have ice at the top, valleys might be grassy, etc...
5. Proper physics engine
6. Fluid dynamics

initialization :
----------------
```bash
git submodule init
git submodule update
```

usage (macos) : 
---------------
Install necessary tools and libraries with homebrew
```bash
brew install llvm cmake
```

Open a terminal in the root of the repo
```bash
chmod a+x build.sh
./build.sh
./build/terrain-generator
```

usage (Linux) : 
---------------
Some standard libraries to install:
```bash
sudo apt-get install cmake libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

Open the directory into a terminal
```bash
chmod a+x build.sh
./build.sh
./build/terrain-generator
```

usage (Windows) :
-----------------
For instance :
* cmake-gui .
* Configure (Choose for example Visual Studio generator)
* Generate
Launch the generated project in your favorite IDE and run it.
