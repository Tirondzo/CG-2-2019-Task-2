# CG/2 Course Task 2

## Requirements
 - C++ Compiler
 - CMake
 - OpenGL 3.3+
 - GLFW3 Library

To install almost everything you can run this: \
`sudo apt install mesa-utils xorg-dev libglu1-mesa-dev cmake make build-essential`

To install GLFW3 with package your manager you can run this: \
`sudo <your package manager> install libglfw3`

Or it may called `libglfw3-dev` on Linux machines

To install GLFW3 from sources:
```sh
sudo apt install unzip cmake make && \
wget "https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip" && \
unzip glfw-3.2.1.zip && \
cd glfw-3.2.1 && \
sudo cmake -G "Unix Makefiles" && \
sudo make && \
sudo make install && \
cd .. && \
sudo rm -f glfw-3.2.1.zip && \
sudo rm -rf glfw-3.2.1
```
More info here: https://www.glfw.org/download.html


## Installation
0. Go to root directory
1. `mkdir build && cd build`
1. `cmake ..`
2. `make`
3. `./main`

## Working
After installation you can add, remove and change files in `src`, `assets` and `shaders` dir. \
If you changed something you can use `make` and everything should be fine. \
But if something is wrong you can try to run `cmake ..` to regenerate cmake file.