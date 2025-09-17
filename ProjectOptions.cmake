function(print_options)
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
  message(STATUS "Developer Mode: ${DEV_MODE}")
  message(STATUS "Enable Tests: ${ENABLE_TESTS}")
  message(STATUS "Enable Sanitizers: ${ENABLE_SANITIZERS}")
  message(STATUS "Enable Undefined Behavior Sanitizer: ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}")
  message(STATUS "Enable Address Sanitizer: ${ENABLE_SANITIZER_ADDRESS}")
  message(STATUS "Enable Leak Sanitizer: ${ENABLE_SANITIZER_LEAK}")
  message(STATUS "Enable Thread Sanitizer: ${ENABLE_SANITIZER_THREAD}")
  message(STATUS "Enable Memory Sanitizer: ${ENABLE_SANITIZER_MEMORY}")
  message(STATUS "Logging without file prefix: ${LOGGING_NO_PREFIX}")
endfunction()

function(
    set_project_options
    project_name)
  if(${LOGGING_NO_PREFIX})
    target_compile_definitions(${project_name} INTERFACE LOGGING_NO_PREFIX=${LOGGING_NO_PREFIX})
  endif()
endfunction()

macro(setup_options)

  # Dev Mode
  option(DEV_MODE "Enable developer mode" OFF)
  if(${DEV_MODE})
    option(ENABLE_SANITIZERS "Enable sanitizers" ON)
    option(ENABLE_TESTS "Enable tests" ON)
    if (NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE "Debug")
    endif()
  else()
    option(ENABLE_SANITIZERS "Enable sanitizers" OFF)
    option(ENABLE_TESTS "Enable tests" OFF)
    if (NOT CMAKE_BUILD_TYPE)
      if (NOT ${DEFAULT_BUILD_TYPE})
        set(CMAKE_BUILD_TYPE "Release")
      else()
        set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE})
      endif()
    endif()
  endif()

  # Determine if this is a release build
  if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(IS_RELEASE_BUILD 1)
  else()
    set(IS_RELEASE_BUILD 0)
  endif()

  # If build type is a release build, then disable logging prefix by default.
  if(${IS_RELEASE_BUILD})
    option(LOGGING_NO_PREFIX "Log without file prefix" ON)
  else()
    option(LOGGING_NO_PREFIX "Log with file prefix" OFF)
  endif()

  # Sanitizers
  if(MSVC)
    option(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable undefined sanitizer" OFF)
  else()
    option(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable undefined sanitizer" ON)
  endif()
  option(ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ON)
  option(ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
  option(ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
  option(ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)

endmacro()
