cmake_minimum_required(VERSION 3.15)
project(bf2c VERSION 0.3.0)

# for correct library locations across platforms
include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD "99")
set(CMAKE_C_STANDARD_REQUIRED ON)
set(DEFAULT_BUILD_TYPE "Release")

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE})
endif()
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(IS_RELEASE_BUILD 1)
else()
  set(IS_RELEASE_BUILD 0)
endif()

# get the current date and set it as a variable
string(TIMESTAMP BUILD_DATE "%Y-%m-%d")

include(cmake/PreventInSourceBuilds.cmake)
include(cmake/Git.cmake)

option(LOGGING_NO_PREFIX "Logging without prefix" ${IS_RELEASE_BUILD})

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

if (BUILD_BINDINGS)
  add_subdirectory(bindings)
endif()
