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
include src/bsoncxx/test/CMakeFiles/test_bson.dir/depend.make

# Include the progress variables for this target.
include src/bsoncxx/test/CMakeFiles/test_bson.dir/progress.make

# Include the compile flags for this target's objects.
include src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make

src/bsoncxx/test/CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.o: ../src/third_party/catch/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/third_party/catch/main.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/third_party/catch/main.cpp > CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/third_party/catch/main.cpp -o CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.s

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_b_date.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_b_date.cpp.o: ../src/bsoncxx/test/bson_b_date.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_b_date.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/bson_b_date.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_b_date.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_b_date.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/bson_b_date.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_b_date.cpp > CMakeFiles/test_bson.dir/bson_b_date.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_b_date.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/bson_b_date.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_b_date.cpp -o CMakeFiles/test_bson.dir/bson_b_date.cpp.s

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_builder.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_builder.cpp.o: ../src/bsoncxx/test/bson_builder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_builder.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/bson_builder.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_builder.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_builder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/bson_builder.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_builder.cpp > CMakeFiles/test_bson.dir/bson_builder.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_builder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/bson_builder.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_builder.cpp -o CMakeFiles/test_bson.dir/bson_builder.cpp.s

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_get_values.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_get_values.cpp.o: ../src/bsoncxx/test/bson_get_values.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_get_values.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/bson_get_values.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_get_values.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_get_values.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/bson_get_values.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_get_values.cpp > CMakeFiles/test_bson.dir/bson_get_values.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_get_values.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/bson_get_values.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_get_values.cpp -o CMakeFiles/test_bson.dir/bson_get_values.cpp.s

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_types.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_types.cpp.o: ../src/bsoncxx/test/bson_types.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_types.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/bson_types.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_types.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_types.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/bson_types.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_types.cpp > CMakeFiles/test_bson.dir/bson_types.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_types.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/bson_types.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_types.cpp -o CMakeFiles/test_bson.dir/bson_types.cpp.s

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_util_itoa.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_util_itoa.cpp.o: ../src/bsoncxx/test/bson_util_itoa.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_util_itoa.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/bson_util_itoa.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_util_itoa.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_util_itoa.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/bson_util_itoa.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_util_itoa.cpp > CMakeFiles/test_bson.dir/bson_util_itoa.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_util_itoa.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/bson_util_itoa.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_util_itoa.cpp -o CMakeFiles/test_bson.dir/bson_util_itoa.cpp.s

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_validate.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_validate.cpp.o: ../src/bsoncxx/test/bson_validate.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_validate.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/bson_validate.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_validate.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_validate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/bson_validate.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_validate.cpp > CMakeFiles/test_bson.dir/bson_validate.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_validate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/bson_validate.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_validate.cpp -o CMakeFiles/test_bson.dir/bson_validate.cpp.s

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_value.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_value.cpp.o: ../src/bsoncxx/test/bson_value.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_value.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/bson_value.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_value.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_value.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/bson_value.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_value.cpp > CMakeFiles/test_bson.dir/bson_value.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_value.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/bson_value.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/bson_value.cpp -o CMakeFiles/test_bson.dir/bson_value.cpp.s

src/bsoncxx/test/CMakeFiles/test_bson.dir/json.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/json.cpp.o: ../src/bsoncxx/test/json.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/json.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/json.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/json.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/json.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/json.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/json.cpp > CMakeFiles/test_bson.dir/json.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/json.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/json.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/json.cpp -o CMakeFiles/test_bson.dir/json.cpp.s

src/bsoncxx/test/CMakeFiles/test_bson.dir/view_or_value.cpp.o: src/bsoncxx/test/CMakeFiles/test_bson.dir/flags.make
src/bsoncxx/test/CMakeFiles/test_bson.dir/view_or_value.cpp.o: ../src/bsoncxx/test/view_or_value.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/bsoncxx/test/CMakeFiles/test_bson.dir/view_or_value.cpp.o"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bson.dir/view_or_value.cpp.o -c /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/view_or_value.cpp

src/bsoncxx/test/CMakeFiles/test_bson.dir/view_or_value.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bson.dir/view_or_value.cpp.i"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/view_or_value.cpp > CMakeFiles/test_bson.dir/view_or_value.cpp.i

src/bsoncxx/test/CMakeFiles/test_bson.dir/view_or_value.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bson.dir/view_or_value.cpp.s"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test/view_or_value.cpp -o CMakeFiles/test_bson.dir/view_or_value.cpp.s

# Object files for target test_bson
test_bson_OBJECTS = \
"CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.o" \
"CMakeFiles/test_bson.dir/bson_b_date.cpp.o" \
"CMakeFiles/test_bson.dir/bson_builder.cpp.o" \
"CMakeFiles/test_bson.dir/bson_get_values.cpp.o" \
"CMakeFiles/test_bson.dir/bson_types.cpp.o" \
"CMakeFiles/test_bson.dir/bson_util_itoa.cpp.o" \
"CMakeFiles/test_bson.dir/bson_validate.cpp.o" \
"CMakeFiles/test_bson.dir/bson_value.cpp.o" \
"CMakeFiles/test_bson.dir/json.cpp.o" \
"CMakeFiles/test_bson.dir/view_or_value.cpp.o"

# External object files for target test_bson
test_bson_EXTERNAL_OBJECTS =

src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/__/__/third_party/catch/main.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_b_date.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_builder.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_get_values.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_types.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_util_itoa.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_validate.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/bson_value.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/json.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/view_or_value.cpp.o
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/build.make
src/bsoncxx/test/test_bson: src/bsoncxx/libbsoncxx-testing.so.0.0.0
src/bsoncxx/test/test_bson: /usr/local/lib/libbson-1.0.so.0.0.0
src/bsoncxx/test/test_bson: src/bsoncxx/test/CMakeFiles/test_bson.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable test_bson"
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_bson.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/bsoncxx/test/CMakeFiles/test_bson.dir/build: src/bsoncxx/test/test_bson

.PHONY : src/bsoncxx/test/CMakeFiles/test_bson.dir/build

src/bsoncxx/test/CMakeFiles/test_bson.dir/clean:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test && $(CMAKE_COMMAND) -P CMakeFiles/test_bson.dir/cmake_clean.cmake
.PHONY : src/bsoncxx/test/CMakeFiles/test_bson.dir/clean

src/bsoncxx/test/CMakeFiles/test_bson.dir/depend:
	cd /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx/test /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test/CMakeFiles/test_bson.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/bsoncxx/test/CMakeFiles/test_bson.dir/depend

