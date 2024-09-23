cmake_minimum_required(VERSION 3.15)
project(bf2c VERSION 0.0.1)

# for correct library locations across platforms
include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Release")

include(cmake/PreventInSourceBuilds.cmake)

# Add interface library for compiler warnings
include(cmake/CompilerWarnings.cmake)
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings WARNINGS_AS_ERRORS "" "" "" "")

add_subdirectory(lib)
add_subdirectory(app)
