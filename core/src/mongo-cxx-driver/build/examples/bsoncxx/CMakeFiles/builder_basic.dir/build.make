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
include examples/bsoncxx/CMakeFiles/builder_basic.dir/depend.make

# Include the progress variables for this target.
include examples/bsoncxx/CMakeFiles/builder_basic.dir/progress.make

# Include the compile flags for this target's objects.
include examples/bsoncxx/CMakeFiles/builder_basic.dir/flags.make

examples/bsoncxx/CMakeFiles/builder_basic.dir/builder_basic.cpp.o: examples/bsoncxx/CMakeFiles/builder_basic.dir/flags.make
examples/bsoncxx/CMakeFiles/builder_basic.dir/builder_basic.cpp.o: ../examples/bsoncxx/builder_basic.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/bsoncxx/CMakeFiles/builder_basic.dir/builder_basic.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/builder_basic.dir/builder_basic.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/examples/bsoncxx/builder_basic.cpp

examples/bsoncxx/CMakeFiles/builder_basic.dir/builder_basic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/builder_basic.dir/builder_basic.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/examples/bsoncxx/builder_basic.cpp > CMakeFiles/builder_basic.dir/builder_basic.cpp.i

examples/bsoncxx/CMakeFiles/builder_basic.dir/builder_basic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/builder_basic.dir/builder_basic.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/examples/bsoncxx/builder_basic.cpp -o CMakeFiles/builder_basic.dir/builder_basic.cpp.s

# Object files for target builder_basic
builder_basic_OBJECTS = \
"CMakeFiles/builder_basic.dir/builder_basic.cpp.o"

# External object files for target builder_basic
builder_basic_EXTERNAL_OBJECTS =

examples/bsoncxx/builder_basic: examples/bsoncxx/CMakeFiles/builder_basic.dir/builder_basic.cpp.o
examples/bsoncxx/builder_basic: examples/bsoncxx/CMakeFiles/builder_basic.dir/build.make
examples/bsoncxx/builder_basic: src/bsoncxx/libbsoncxx.so.0.0.0
examples/bsoncxx/builder_basic: examples/bsoncxx/CMakeFiles/builder_basic.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable builder_basic"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/builder_basic.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/bsoncxx/CMakeFiles/builder_basic.dir/build: examples/bsoncxx/builder_basic

.PHONY : examples/bsoncxx/CMakeFiles/builder_basic.dir/build

examples/bsoncxx/CMakeFiles/builder_basic.dir/clean:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx && $(CMAKE_COMMAND) -P CMakeFiles/builder_basic.dir/cmake_clean.cmake
.PHONY : examples/bsoncxx/CMakeFiles/builder_basic.dir/clean

examples/bsoncxx/CMakeFiles/builder_basic.dir/depend:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/examples/bsoncxx /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/examples/bsoncxx/CMakeFiles/builder_basic.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/bsoncxx/CMakeFiles/builder_basic.dir/depend

