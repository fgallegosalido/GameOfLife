# Game of Life
Implementation of the famous John Conway's Game of Life in C++ using SFML

# Compilation
Clone the repo and compile with the following commands:

```bash
$> mkdir build
$> cd build
$> cmake ..
$> cmake --build .
```

# Requirements
These are the versions the project is compiled with, but lower versions may work:

* GCC 9.3.0
* CMake 3.17.1
* Conan 1.24.1

The following are the dependencies, but they shoud be managed by conan:

* Boost 1.72.0
* Catch2 2.12.1
* SFML 2.5.1
* spdlog 1.5.0

# Instructions
Once the program is running, ther following keys are available:

* S: Start/Stop the animation
* C: Clear the grid
* Up: Increase the speed of the animation
* Down: Decrease the speed of the animation
* Right: Next state of the grid
* Q/Esc: Close the program
* Left click: Kill/Revive the pointed cell
