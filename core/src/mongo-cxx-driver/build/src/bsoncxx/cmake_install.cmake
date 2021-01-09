# Install script for directory: /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/bsoncxx/v_noabi" TYPE DIRECTORY FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/src/bsoncxx" FILES_MATCHING REGEX "/[^/]*\\.hpp$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/bsoncxx/v_noabi/bsoncxx/config" TYPE FILE FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/config/export.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libbsoncxx-0.0.0" TYPE FILE FILES
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/libbsoncxx-config.cmake"
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/libbsoncxx-config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so._noabi"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "/usr/local/lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/libbsoncxx.so.0.0.0"
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/libbsoncxx.so._noabi"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so.0.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so._noabi"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/usr/local/lib:"
           NEW_RPATH "/usr/local/lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so"
         RPATH "/usr/local/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/libbsoncxx.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "/usr/local/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/bsoncxx-0.0.0/bsoncxx_targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/bsoncxx-0.0.0/bsoncxx_targets.cmake"
         "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/CMakeFiles/Export/lib/cmake/bsoncxx-0.0.0/bsoncxx_targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/bsoncxx-0.0.0/bsoncxx_targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/bsoncxx-0.0.0/bsoncxx_targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/bsoncxx-0.0.0" TYPE FILE FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/CMakeFiles/Export/lib/cmake/bsoncxx-0.0.0/bsoncxx_targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/bsoncxx-0.0.0" TYPE FILE FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/CMakeFiles/Export/lib/cmake/bsoncxx-0.0.0/bsoncxx_targets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/bsoncxx-0.0.0" TYPE FILE FILES
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/bsoncxx-config-version.cmake"
    "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/bsoncxx-config.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/third_party/cmake_install.cmake")
  include("/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/config/cmake_install.cmake")
  include("/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/src/bsoncxx/test/cmake_install.cmake")

endif()

