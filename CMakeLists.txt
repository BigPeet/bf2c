cmake_minimum_required(VERSION 3.15)
project(bf2c VERSION 0.2.0)

# for correct library locations across platforms
include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD "99")
set(CMAKE_C_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(cmake/PreventInSourceBuilds.cmake)


# If build type is release then disable logging prefix
if((NOT LOGGING_NO_PREFIX) AND (${CMAKE_BUILD_TYPE} STREQUAL "Release"))
  set(LOGGING_NO_PREFIX ON)
endif()

# Project specific definitions TODO: move into more suitable file
if(${LOGGING_NO_PREFIX})
  add_definitions(-DLOGGING_NO_PREFIX=${LOGGING_NO_PREFIX})
endif()

# Add interface library for compiler warnings
include(cmake/CompilerWarnings.cmake)
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings WARNINGS_AS_ERRORS "" "" "" "")

add_subdirectory(lib)
add_subdirectory(app)
