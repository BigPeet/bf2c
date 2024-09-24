add_library(logging INTERFACE)
target_include_directories(logging INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
