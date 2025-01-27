# BF2C library

set (BF2C_SOURCE_FILES
  src/token.c
  src/parser.c
  src/command.c
  src/program.c
  src/c_emitter.c
  )

add_library(bf2c_lib STATIC ${BF2C_SOURCE_FILES})

target_link_libraries(bf2c_lib PRIVATE project_warnings core)
target_include_directories(bf2c_lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_compile_features(bf2c_lib PUBLIC c_std_99)
