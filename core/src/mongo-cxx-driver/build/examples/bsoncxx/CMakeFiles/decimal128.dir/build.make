# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build

# Include any dependencies generated for this target.
include examples/bsoncxx/CMakeFiles/decimal128.dir/depend.make

# Include the progress variables for this target.
include examples/bsoncxx/CMakeFiles/decimal128.dir/progress.make

# Include the compile flags for this target's objects.
include examples/bsoncxx/CMakeFiles/decimal128.dir/flags.make

examples/bsoncxx/CMakeFiles/decimal128.dir/decimal128.cpp.o: examples/bsoncxx/CMakeFiles/decimal128.dir/flags.make
examples/bsoncxx/CMakeFiles/decimal128.dir/decimal128.cpp.o: ../examples/bsoncxx/decimal128.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/bsoncxx/CMakeFiles/decimal128.dir/decimal128.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decimal128.dir/decimal128.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/examples/bsoncxx/decimal128.cpp

examples/bsoncxx/CMakeFiles/decimal128.dir/decimal128.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decimal128.dir/decimal128.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/examples/bsoncxx/decimal128.cpp > CMakeFiles/decimal128.dir/decimal128.cpp.i

examples/bsoncxx/CMakeFiles/decimal128.dir/decimal128.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decimal128.dir/decimal128.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/examples/bsoncxx/decimal128.cpp -o CMakeFiles/decimal128.dir/decimal128.cpp.s

# Object files for target decimal128
decimal128_OBJECTS = \
"CMakeFiles/decimal128.dir/decimal128.cpp.o"

# External object files for target decimal128
decimal128_EXTERNAL_OBJECTS =

examples/bsoncxx/decimal128: examples/bsoncxx/CMakeFiles/decimal128.dir/decimal128.cpp.o
examples/bsoncxx/decimal128: examples/bsoncxx/CMakeFiles/decimal128.dir/build.make
examples/bsoncxx/decimal128: src/bsoncxx/libbsoncxx.so.0.0.0
examples/bsoncxx/decimal128: examples/bsoncxx/CMakeFiles/decimal128.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable decimal128"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/decimal128.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/bsoncxx/CMakeFiles/decimal128.dir/build: examples/bsoncxx/decimal128

.PHONY : examples/bsoncxx/CMakeFiles/decimal128.dir/build

examples/bsoncxx/CMakeFiles/decimal128.dir/clean:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && $(CMAKE_COMMAND) -P CMakeFiles/decimal128.dir/cmake_clean.cmake
.PHONY : examples/bsoncxx/CMakeFiles/decimal128.dir/clean

examples/bsoncxx/CMakeFiles/decimal128.dir/depend:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/examples/bsoncxx /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx/CMakeFiles/decimal128.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/bsoncxx/CMakeFiles/decimal128.dir/depend

