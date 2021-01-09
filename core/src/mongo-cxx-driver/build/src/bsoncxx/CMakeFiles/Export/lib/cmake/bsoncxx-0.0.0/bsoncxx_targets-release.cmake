#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mongo::bsoncxx_shared" for configuration "Release"
set_property(TARGET mongo::bsoncxx_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(mongo::bsoncxx_shared PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "mongo::bson_shared"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libbsoncxx.so.0.0.0"
  IMPORTED_SONAME_RELEASE "libbsoncxx.so._noabi"
  )

list(APPEND _IMPORT_CHECK_TARGETS mongo::bsoncxx_shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_mongo::bsoncxx_shared "${_IMPORT_PREFIX}/lib/libbsoncxx.so.0.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
