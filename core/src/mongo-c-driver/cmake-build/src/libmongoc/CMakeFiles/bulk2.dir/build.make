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
include src/libmongoc/CMakeFiles/bulk2.dir/depend.make

# Include the progress variables for this target.
include src/libmongoc/CMakeFiles/bulk2.dir/progress.make

# Include the compile flags for this target's objects.
include src/libmongoc/CMakeFiles/bulk2.dir/flags.make

src/libmongoc/CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.o: src/libmongoc/CMakeFiles/bulk2.dir/flags.make
src/libmongoc/CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.o: ../src/libmongoc/examples/bulk/bulk2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/libmongoc/CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.o   -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libmongoc/examples/bulk/bulk2.c

src/libmongoc/CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libmongoc/examples/bulk/bulk2.c > CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.i

src/libmongoc/CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libmongoc/examples/bulk/bulk2.c -o CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.s

# Object files for target bulk2
bulk2_OBJECTS = \
"CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.o"

# External object files for target bulk2
bulk2_EXTERNAL_OBJECTS =

src/libmongoc/bulk2: src/libmongoc/CMakeFiles/bulk2.dir/examples/bulk/bulk2.c.o
src/libmongoc/bulk2: src/libmongoc/CMakeFiles/bulk2.dir/build.make
src/libmongoc/bulk2: src/libmongoc/libmongoc-1.0.so.0.0.0
src/libmongoc/bulk2: /usr/lib/x86_64-linux-gnu/libsasl2.so
src/libmongoc/bulk2: /usr/lib/x86_64-linux-gnu/libssl.so
src/libmongoc/bulk2: /usr/lib/x86_64-linux-gnu/libcrypto.so
src/libmongoc/bulk2: /usr/lib/x86_64-linux-gnu/libsnappy.so
src/libmongoc/bulk2: /usr/lib/x86_64-linux-gnu/libz.so
src/libmongoc/bulk2: /usr/lib/x86_64-linux-gnu/libicuuc.so
src/libmongoc/bulk2: src/libbson/libbson-1.0.so.0.0.0
src/libmongoc/bulk2: src/libmongoc/CMakeFiles/bulk2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bulk2"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bulk2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/libmongoc/CMakeFiles/bulk2.dir/build: src/libmongoc/bulk2

.PHONY : src/libmongoc/CMakeFiles/bulk2.dir/build

src/libmongoc/CMakeFiles/bulk2.dir/clean:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc && $(CMAKE_COMMAND) -P CMakeFiles/bulk2.dir/cmake_clean.cmake
.PHONY : src/libmongoc/CMakeFiles/bulk2.dir/clean

src/libmongoc/CMakeFiles/bulk2.dir/depend:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/src/libmongoc /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-c-driver/cmake-build/src/libmongoc/CMakeFiles/bulk2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/libmongoc/CMakeFiles/bulk2.dir/depend

