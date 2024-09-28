set(CLI_SOURCE_FILES
  src/cli.c
  src/option.c
  src/error_codes.c
  )
add_library(cli STATIC ${CLI_SOURCE_FILES})

target_link_libraries(cli PRIVATE project_warnings core)
target_include_directories(cli PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_compile_features(cli PUBLIC c_std_99)
