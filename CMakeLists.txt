cmake_minimum_required(VERSION 3.15)
project(bf2c VERSION 0.2.0)

# for correct library locations across platforms
include(GNUInstallDirs)
# prevent in-source builds
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD "99")
set(CMAKE_C_STANDARD_REQUIRED ON)

# Various Project options
include(ProjectOptions.cmake)
setup_options()

# Add interface library for compiler warnings
include(cmake/CompilerWarnings.cmake)
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings WARNINGS_AS_ERRORS "" "" "" "")

# Add interface library for various options
add_library(project_options INTERFACE)

# Project specific definitions
if(${LOGGING_NO_PREFIX})
  target_compile_definitions(project_options INTERFACE LOGGING_NO_PREFIX=${LOGGING_NO_PREFIX})
endif()

# Sanitizers
if(${ENABLE_SANITIZERS})
  include(cmake/Sanitizers.cmake)
  enable_sanitizers(project_options
    ${ENABLE_SANITIZER_ADDRESS}
    ${ENABLE_SANITIZER_LEAK}
    ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
    ${ENABLE_SANITIZER_THREAD}
    ${ENABLE_SANITIZER_MEMORY}
    )
endif()

# Testing
if (${ENABLE_TESTS})
  enable_testing()
  add_subdirectory(tests)
endif()

add_subdirectory(lib)
add_subdirectory(app)
