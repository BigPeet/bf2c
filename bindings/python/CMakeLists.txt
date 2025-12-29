# BF2C Python module (SO)
add_library(py_bf2c SHARED src/py_bfc2.c)

# Remove the 'lib' prefix from the shared library name on Linux/Mac
set_target_properties(py_bf2c PROPERTIES PREFIX "")

target_include_directories(py_bf2c PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(py_bf2c PRIVATE core bf2c_lib)

set(PYTHON_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/bf2c.py)

# Copy the Python files to the build directory next to the shared library
add_custom_command(TARGET py_bf2c POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PYTHON_FILES}
    $<TARGET_FILE_DIR:py_bf2c>)
