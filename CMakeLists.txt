cmake_minimum_required(VERSION 3.30)
project(MyLogger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure gcc is used
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(WARNING "This project is designed for GCC compiler")
endif()

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

# Library targets
add_library(mylogger_static STATIC ${SOURCES} ${HEADERS})
target_include_directories(mylogger_static PUBLIC include)
set_target_properties(mylogger_static PROPERTIES OUTPUT_NAME "mylogger")

add_library(mylogger_shared SHARED ${SOURCES} ${HEADERS})
target_include_directories(mylogger_shared PUBLIC include)
set_target_properties(mylogger_shared PROPERTIES OUTPUT_NAME "mylogger")

if(WIN32)
    target_compile_definitions(mylogger_shared PRIVATE MYLOGGER_EXPORTS MYLOGGER_SHARED)
endif()

# Tests
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# App
option(BUILD_APP "Build application" ON)

if(BUILD_APP)
    add_subdirectory(app)
endif()

