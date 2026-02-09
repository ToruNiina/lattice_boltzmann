add_executable(lbm main.cpp)

target_compile_features(lbm PRIVATE cxx_std_20)
target_include_directories(lbm PRIVATE ${PROJECT_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})
target_link_libraries(lbm PRIVATE ${SDL2_LIBRARIES})
