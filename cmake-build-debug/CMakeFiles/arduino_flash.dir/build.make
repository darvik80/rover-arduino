# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/darvik/Projects/cpp/rover/lib/arduino

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/darvik/Projects/cpp/rover/lib/arduino/cmake-build-debug

# Utility rule file for arduino_flash.

# Include the progress variables for this target.
include CMakeFiles/arduino_flash.dir/progress.make

CMakeFiles/arduino_flash:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/darvik/Projects/cpp/rover/lib/arduino/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Uploading arduino target"
	/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude "-C/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf" -v -patmega328p -carduino "-P/dev/cu.usbserial-14330" -b115200 -D -Uflash:w:"/Users/darvik/Projects/cpp/rover/lib/arduino/cmake-build-debug/arduino.hex":i -Ueeprom:w:"/Users/darvik/Projects/cpp/rover/lib/arduino/cmake-build-debug/arduino.eep":i

arduino_flash: CMakeFiles/arduino_flash
arduino_flash: CMakeFiles/arduino_flash.dir/build.make

.PHONY : arduino_flash

# Rule to build all files generated by this target.
CMakeFiles/arduino_flash.dir/build: arduino_flash

.PHONY : CMakeFiles/arduino_flash.dir/build

CMakeFiles/arduino_flash.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/arduino_flash.dir/cmake_clean.cmake
.PHONY : CMakeFiles/arduino_flash.dir/clean

CMakeFiles/arduino_flash.dir/depend:
	cd /Users/darvik/Projects/cpp/rover/lib/arduino/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/darvik/Projects/cpp/rover/lib/arduino /Users/darvik/Projects/cpp/rover/lib/arduino /Users/darvik/Projects/cpp/rover/lib/arduino/cmake-build-debug /Users/darvik/Projects/cpp/rover/lib/arduino/cmake-build-debug /Users/darvik/Projects/cpp/rover/lib/arduino/cmake-build-debug/CMakeFiles/arduino_flash.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/arduino_flash.dir/depend
