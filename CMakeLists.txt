cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME arduino)
project(${PROJECT_NAME} LANGUAGES C CXX ASM)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB HDR_FILES ${PROJECT_SOURCE_DIR}/*.h)

arduino_cmake_project(${PROJECT_NAME} BOARD_NAME uno)

add_arduino_executable(${PROJECT_NAME} ${SRC_FILES})

find_arduino_library(wire Wire)
link_arduino_library(${PROJECT_NAME} wire)

set_target_upload_port(${PROJECT_NAME} "/dev/cu.usbserial-14330")
