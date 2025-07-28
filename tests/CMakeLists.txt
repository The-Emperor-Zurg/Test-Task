cmake_minimum_required(VERSION 3.30)

add_executable(tests main.cpp unit_tests.cpp)

# Include directories
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link with static library
target_link_libraries(tests mylogger_static)

# Set output name
set_target_properties(tests PROPERTIES OUTPUT_NAME "logger_test")