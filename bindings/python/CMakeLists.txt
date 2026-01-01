
# Define the Python Bindings target which requires a custom command to build
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

  set(WHEEL ${CMAKE_BINARY_DIR}/dist/bf2c-${PROJECT_VERSION}-cp${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}-cp${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}-linux_x86_64.whl)
  set(PYTHON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/setup.py
    ${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bf2c/__init__.py
  )

  # Add custom command to generate Python bindings
  add_custom_command(OUTPUT ${WHEEL}
    COMMAND ${CMAKE_COMMAND} -E env
      BF2C_LIB_DIR=${CMAKE_BINARY_DIR}/lib/bf2c
      BF2C_INCLUDE_DIR=$<TARGET_PROPERTY:bf2c_lib,INTERFACE_INCLUDE_DIRECTORIES>
      CORE_LIB_DIR=${CMAKE_BINARY_DIR}/lib/core
      CORE_INCLUDE_DIR=$<TARGET_PROPERTY:core,INTERFACE_INCLUDE_DIRECTORIES>
      BF2C_VERSION=${PROJECT_VERSION}
      ${Python3_EXECUTABLE} -m build -o ${CMAKE_BINARY_DIR}/dist
    COMMAND rm -r ${CMAKE_CURRENT_SOURCE_DIR}/src/bf2c.egg-info
    BYPRODUCTS ${CMAKE_BINARY_DIR}/dist/bf2c-${PROJECT_VERSION}.tar.gz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/bf2c/py_bf2c.c
    DEPENDS bf2c_lib core ${PYTHON_FILES}
    COMMENT "Building Python wheel..."
    VERBATIM
  )

  # Add custom target which depends on the custom command
  add_custom_target(bf2c_python_bindings
    DEPENDS ${WHEEL}
  )

endif()
