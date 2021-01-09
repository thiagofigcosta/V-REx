# Install script for directory: /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/mongocxx

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mongocxx/v_noabi" TYPE DIRECTORY FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/mongocxx" FILES_MATCHING REGEX "/[^/]*\\.hpp$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mongocxx/v_noabi/mongocxx/config" TYPE FILE FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/config/export.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libmongocxx-0.0.0" TYPE FILE FILES
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/libmongocxx-config.cmake"
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/libmongocxx-config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so._noabi"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "/usr/local/lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/libmongocxx.so.0.0.0"
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/libmongocxx.so._noabi"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so._noabi"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx:/usr/local/lib:"
           NEW_RPATH "/usr/local/lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so"
         RPATH "/usr/local/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/libmongocxx.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so"
         OLD_RPATH "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx:/usr/local/lib:"
         NEW_RPATH "/usr/local/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libmongocxx.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/mongocxx-0.0.0/mongocxx_targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/mongocxx-0.0.0/mongocxx_targets.cmake"
         "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/CMakeFiles/Export/lib/cmake/mongocxx-0.0.0/mongocxx_targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/mongocxx-0.0.0/mongocxx_targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/mongocxx-0.0.0/mongocxx_targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/mongocxx-0.0.0" TYPE FILE FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/CMakeFiles/Export/lib/cmake/mongocxx-0.0.0/mongocxx_targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/mongocxx-0.0.0" TYPE FILE FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/CMakeFiles/Export/lib/cmake/mongocxx-0.0.0/mongocxx_targets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/mongocxx-0.0.0" TYPE FILE FILES
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/mongocxx-config-version.cmake"
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/mongocxx-config.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/config/cmake_install.cmake")
  include("/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/mongocxx/test/cmake_install.cmake")

endif()

