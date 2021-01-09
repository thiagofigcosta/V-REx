# Install script for directory: /home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/generate_uninstall

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  
      if (EXISTS ${CMAKE_BINARY_DIR}/src/bsoncxx/third_party/EP_mnmlstc_core-prefix/src/EP_mnmlstc_core-build/install_manifest.txt)
         file (
            STRINGS
               ${CMAKE_BINARY_DIR}/src/bsoncxx/third_party/EP_mnmlstc_core-prefix/src/EP_mnmlstc_core-build/install_manifest.txt
               ADDL_MANIFEST_FILES
         )
         list (APPEND CMAKE_INSTALL_MANIFEST_FILES ${ADDL_MANIFEST_FILES})
      endif ()
      string(REPLACE ";" "\n" MONGOCXX_INSTALL_MANIFEST_CONTENT
         "${CMAKE_INSTALL_MANIFEST_FILES}")
      file(WRITE "mongocxx_install_manifest.txt"
         "${MONGOCXX_INSTALL_MANIFEST_CONTENT}")
      execute_process (
         COMMAND
            find /usr/local -type d -empty -delete
      )
      execute_process (
         COMMAND
            /usr/bin/cmake -E env
            "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/etc/generate-uninstall.sh"
            mongocxx_install_manifest.txt
            /usr/local
         OUTPUT_FILE
            "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/generate_uninstall/uninstall.sh"
      )
   
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mongo-cxx-driver" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/home/computahackerboy/Desktop/Programming/V-REx/core/src/mongo-cxx-driver/build/generate_uninstall/uninstall.sh")
endif()

