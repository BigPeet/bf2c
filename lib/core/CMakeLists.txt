# Core Library
add_library(core STATIC src/logging.c src/vector.c)
target_include_directories(core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_definitions(core PUBLIC CORE_VECTOR_DECLARE_BASIC_TYPES)
target_link_libraries(core PRIVATE project_warnings project_options)
