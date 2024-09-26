# Util Library
add_library(core_util STATIC src/util/logging.c)
target_include_directories(core_util PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# Result Library
add_library(core_result STATIC src/result/result.c)
target_include_directories(core_result PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(core_result PRIVATE core_util)
