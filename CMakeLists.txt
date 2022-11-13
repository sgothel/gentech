cmake_minimum_required (VERSION 3.13.0 FATAL_ERROR)
project (gentech LANGUAGES C CXX)

if(${CMAKE_VERSION} VERSION_LESS "3.18.0")
    message(STATUS "CMake ${CMAKE_VERSION}, please consider to switch to CMake 3.18.0 or later")
else()
    message(STATUS "CMake ${CMAKE_VERSION} >= 3.18.0, good.")
endif()

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

# set(CMAKE_MESSAGE_LOG_LEVEL DEBUG)

if(NOT DEFINED DONT_USE_RTTI)
    set(DONT_USE_RTTI ON)
    message(STATUS "${PROJECT_NAME} DONT_USE_RTTI ${DONT_USE_RTTI} (gentech default)")
else()
    message(STATUS "${PROJECT_NAME} DONT_USE_RTTI ${DONT_USE_RTTI} (user)")
endif()

include(jaulib/JaulibSetup.cmake)
JaulibSetup()

# this is the library version, derived from git tag and revision
set (gentech_VERSION_MAJOR      ${VERSION_MAJOR})
set (gentech_VERSION_MINOR      ${VERSION_MINOR})
set (gentech_VERSION_PATCH      ${VERSION_PATCH})
set (gentech_VERSION_COMMITS    ${VERSION_COMMITS})
set (gentech_VERSION_SHA1       ${VERSION_SHA1})
set (gentech_VERSION_SHA1_SHORT ${VERSION_SHA1_SHORT})
set (gentech_GIT_DIRTY          ${GIT_WORKDIR_DIRTY})
set (gentech_VERSION_LONG       ${VERSION_LONG})
set (gentech_VERSION_SHORT      ${VERSION_SHORT})
set (gentech_VERSION_API        ${VERSION_API})

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/src/gentech/version.cpp.in
                ${CMAKE_CURRENT_BINARY_DIR}/src/version.cpp)

set (CMAKE_SWIG_FLAGS "")

set (gentech_DOCS_DIR documentation)

find_path (SYSTEM_USR_DIR "stdlib.h")
include_directories (${SYSTEM_USR_DIR})

if (BUILD_TESTING)
  add_subdirectory (jaulib)
  enable_testing ()
endif(BUILD_TESTING)

add_subdirectory (src/gen-river)
add_subdirectory (src/gen-salesman)

# add a target to generate API documentation with Doxygen
find_package (Doxygen)
if (DOXYGEN_FOUND)
  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.cpp.out @ONLY)
  add_custom_command (OUTPUT ${gentech_DOCS_DIR}/cpp
      COMMAND ${CMAKE_COMMAND} -E make_directory ${gentech_DOCS_DIR}
      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.cpp.out
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
  add_custom_target(doc
    DEPENDS ${gentech_DOCS_DIR}/cpp)
endif (DOXYGEN_FOUND)

