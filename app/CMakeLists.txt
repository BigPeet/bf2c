set(APP_SOURCE_FILES src/main.c)

add_executable(${PROJECT_NAME} ${APP_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings cli core)
target_compile_features(${PROJECT_NAME} PUBLIC c_std_99)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
