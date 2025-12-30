find_package(Python3 COMPONENTS Interpreter Development)
if (Python3_FOUND)
  execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import build"
    RESULT_VARIABLE EXIT_CODE
    OUTPUT_QUIET
    ERROR_QUIET
    )
  if (NOT ${EXIT_CODE} EQUAL 0)
    message(STATUS "The 'build' module is required to build the Python bindings.")
  else()
    add_subdirectory(python)
  endif()
else()
  message(STATUS "Python3 not found. Skipping generation of Python bindings.")
endif()
