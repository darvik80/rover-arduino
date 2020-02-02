### Build based on Arduino-CMake-NG
link: https://github.com/arduino-cmake/Arduino-CMake-NG

 **You should then call CMake (either through cmd, cmake-gui or an IDE if it supports that), passing it the argument -DCMAKE_TOOLCHAIN_FILE=[project_path]/cmake/Arduino-Toolchain.cmake where [project_path] is substituted by the project's full path. This is what allows cmake to use our framework.**