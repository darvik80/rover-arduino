cmake_minimum_required(VERSION 3.8.2)
#set(CMAKE_CXX_STANDARD 03)
set(PROJECT_NAME arduino)
project(${PROJECT_NAME} LANGUAGES C CXX ASM)

file(GLOB SRC_FILES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/utils/*.cpp
        ${PROJECT_SOURCE_DIR}/utils/stream/*.cpp
        ${PROJECT_SOURCE_DIR}/utils/memory/*.cpp
        ${PROJECT_SOURCE_DIR}/protocol/*.cpp
)
file(GLOB HDR_FILES
        ${PROJECT_SOURCE_DIR}/*.h
        ${PROJECT_SOURCE_DIR}/utils/*.h
        ${PROJECT_SOURCE_DIR}/utils/stream/*.h
        ${PROJECT_SOURCE_DIR}/utils/memory/*.h
        ${PROJECT_SOURCE_DIR}/protocol/*.h
)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/etl/profiles)

add_definitions(-DPROFILE_ARDUINO)
add_definitions(-DETL_NO_STL)

arduino_cmake_project(${PROJECT_NAME} BOARD_NAME uno)

add_arduino_executable(${PROJECT_NAME} ${SRC_FILES})

find_arduino_library(wire Wire)
link_arduino_library(${PROJECT_NAME} wire)

set_target_upload_port(${PROJECT_NAME} "/dev/cu.usbserial-14330")
