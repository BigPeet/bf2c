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
      set(CMAKE_BUILD_TYPE "Release")
    endif()
  endif()

  # If build type is not debug then disable logging prefix by default.
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    option(LOGGING_NO_PREFIX "Log without file prefix" OFF)
  else()
    option(LOGGING_NO_PREFIX "Log without file prefix" ON)
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
