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

# Utility rule file for distcheck.

# Include the progress variables for this target.
include CMakeFiles/distcheck.dir/progress.make

CMakeFiles/distcheck:
	/usr/bin/cmake -D CMAKE_MODULE_PATH=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/cmake/make_dist -D CMAKE_PREFIX_PATH= -D PACKAGE_PREFIX=mongo-cxx-driver-r0.0.0 -D CMAKE_CXX_FLAGS= -P /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/cmake/make_dist/MakeDistCheck.cmake

distcheck: CMakeFiles/distcheck
distcheck: CMakeFiles/distcheck.dir/build.make

.PHONY : distcheck

# Rule to build all files generated by this target.
CMakeFiles/distcheck.dir/build: distcheck

.PHONY : CMakeFiles/distcheck.dir/build

CMakeFiles/distcheck.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/distcheck.dir/cmake_clean.cmake
.PHONY : CMakeFiles/distcheck.dir/clean

CMakeFiles/distcheck.dir/depend:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles/distcheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/distcheck.dir/depend

