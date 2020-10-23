CMAKE_MINIMUM_REQUIRED(VERSION 3.0 FATAL_ERROR)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(CNPY)

SET( CMAKE_CXX_STANDARD 11 )

option(ENABLE_STATIC "Build static (.a) library" ON)

find_package(ZLIB REQUIRED)

include_directories(${ZLIB_INCLUDE_DIRS})

if(ENABLE_STATIC)
  add_library(cnpy STATIC "cnpy.cpp")
  set_target_properties(cnpy PROPERTIES OUTPUT_NAME "cnpy")
  target_link_libraries(cnpy ${ZLIB_LIBRARIES})
  install(TARGETS cnpy DESTINATION lib)
else(ENABLE_STATIC)
  add_library(cnpy SHARED "cnpy.cpp")
  target_link_libraries(cnpy ${ZLIB_LIBRARIES})
  install(TARGETS "cnpy" DESTINATION lib PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif(ENABLE_STATIC)

install(FILES "cnpy.h" DESTINATION include)
install(FILES "mat2npz" "npy2mat" "npz2mat" DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

add_executable(example1 example1.cpp)
target_link_libraries(example1 cnpy)
