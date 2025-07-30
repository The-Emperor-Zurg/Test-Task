cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(../include)

file(GLOB_RECURSE APP_SOURCES src/*.cpp)
file(GLOB_RECURSE APP_HEADERS include/*.h)

add_executable(logger_app ${APP_SOURCES} ${APP_HEADERS})

target_link_libraries(logger_app mylogger_static)

set_target_properties(logger_app PROPERTIES OUTPUT_NAME "logger_app")

find_package(Threads REQUIRED)
target_link_libraries(logger_app Threads::Threads)