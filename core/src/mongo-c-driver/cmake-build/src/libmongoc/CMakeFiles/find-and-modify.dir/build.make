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
include src/libmongoc/CMakeFiles/find-and-modify.dir/depend.make

# Include the progress variables for this target.
include src/libmongoc/CMakeFiles/find-and-modify.dir/progress.make

# Include the compile flags for this target's objects.
include src/libmongoc/CMakeFiles/find-and-modify.dir/flags.make

src/libmongoc/CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.o: src/libmongoc/CMakeFiles/find-and-modify.dir/flags.make
src/libmongoc/CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.o: ../src/libmongoc/examples/find-and-modify.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/libmongoc/CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.o   -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libmongoc/examples/find-and-modify.c

src/libmongoc/CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libmongoc/examples/find-and-modify.c > CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.i

src/libmongoc/CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libmongoc/examples/find-and-modify.c -o CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.s

# Object files for target find-and-modify
find__and__modify_OBJECTS = \
"CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.o"

# External object files for target find-and-modify
find__and__modify_EXTERNAL_OBJECTS =

src/libmongoc/find-and-modify: src/libmongoc/CMakeFiles/find-and-modify.dir/examples/find-and-modify.c.o
src/libmongoc/find-and-modify: src/libmongoc/CMakeFiles/find-and-modify.dir/build.make
src/libmongoc/find-and-modify: src/libmongoc/libmongoc-1.0.so.0.0.0
src/libmongoc/find-and-modify: /usr/lib/x86_64-linux-gnu/libsasl2.so
src/libmongoc/find-and-modify: /usr/lib/x86_64-linux-gnu/libssl.so
src/libmongoc/find-and-modify: /usr/lib/x86_64-linux-gnu/libcrypto.so
src/libmongoc/find-and-modify: /usr/lib/x86_64-linux-gnu/libsnappy.so
src/libmongoc/find-and-modify: /usr/lib/x86_64-linux-gnu/libz.so
src/libmongoc/find-and-modify: /usr/lib/x86_64-linux-gnu/libicuuc.so
src/libmongoc/find-and-modify: src/libbson/libbson-1.0.so.0.0.0
src/libmongoc/find-and-modify: src/libmongoc/CMakeFiles/find-and-modify.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable find-and-modify"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/find-and-modify.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/libmongoc/CMakeFiles/find-and-modify.dir/build: src/libmongoc/find-and-modify

.PHONY : src/libmongoc/CMakeFiles/find-and-modify.dir/build

src/libmongoc/CMakeFiles/find-and-modify.dir/clean:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && $(CMAKE_COMMAND) -P CMakeFiles/find-and-modify.dir/cmake_clean.cmake
.PHONY : src/libmongoc/CMakeFiles/find-and-modify.dir/clean

src/libmongoc/CMakeFiles/find-and-modify.dir/depend:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libmongoc /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc/CMakeFiles/find-and-modify.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/libmongoc/CMakeFiles/find-and-modify.dir/depend

