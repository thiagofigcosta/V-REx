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
CMAKE_SOURCE_DIR = /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build

# Include any dependencies generated for this target.
include src/libbson/CMakeFiles/bcon-col-view.dir/depend.make

# Include the progress variables for this target.
include src/libbson/CMakeFiles/bcon-col-view.dir/progress.make

# Include the compile flags for this target's objects.
include src/libbson/CMakeFiles/bcon-col-view.dir/flags.make

src/libbson/CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.o: src/libbson/CMakeFiles/bcon-col-view.dir/flags.make
src/libbson/CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.o: ../src/libbson/examples/bcon-col-view.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/libbson/CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libbson && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.o   -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libbson/examples/bcon-col-view.c

src/libbson/CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libbson && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libbson/examples/bcon-col-view.c > CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.i

src/libbson/CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libbson && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libbson/examples/bcon-col-view.c -o CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.s

# Object files for target bcon-col-view
bcon__col__view_OBJECTS = \
"CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.o"

# External object files for target bcon-col-view
bcon__col__view_EXTERNAL_OBJECTS =

src/libbson/bcon-col-view: src/libbson/CMakeFiles/bcon-col-view.dir/examples/bcon-col-view.c.o
src/libbson/bcon-col-view: src/libbson/CMakeFiles/bcon-col-view.dir/build.make
src/libbson/bcon-col-view: src/libbson/libbson-1.0.so.0.0.0
src/libbson/bcon-col-view: src/libbson/CMakeFiles/bcon-col-view.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bcon-col-view"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libbson && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bcon-col-view.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/libbson/CMakeFiles/bcon-col-view.dir/build: src/libbson/bcon-col-view

.PHONY : src/libbson/CMakeFiles/bcon-col-view.dir/build

src/libbson/CMakeFiles/bcon-col-view.dir/clean:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libbson && $(CMAKE_COMMAND) -P CMakeFiles/bcon-col-view.dir/cmake_clean.cmake
.PHONY : src/libbson/CMakeFiles/bcon-col-view.dir/clean

src/libbson/CMakeFiles/bcon-col-view.dir/depend:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libbson /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libbson /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libbson/CMakeFiles/bcon-col-view.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/libbson/CMakeFiles/bcon-col-view.dir/depend

