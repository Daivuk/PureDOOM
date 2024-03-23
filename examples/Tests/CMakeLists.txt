cmake_minimum_required(VERSION 3.0)

# Project
project(pd_tests)

add_definitions(-DDOOM_FAST_TICK)
add_definitions(
    -DDOOM_IMPLEMENT_PRINT
    -DDOOM_IMPLEMENT_MALLOC
    -DDOOM_IMPLEMENT_FILE_IO
    -DDOOM_IMPLEMENT_SOCKETS
    -DDOOM_IMPLEMENT_GETTIME
    -DDOOM_IMPLEMENT_EXIT
    -DDOOM_IMPLEMENT_GETENV
)

# Sources
file(GLOB dptests_src_files tests.c)
list(APPEND dptests_includes PUBLIC ../../)

# Executable
add_executable(${PROJECT_NAME} ${dptests_src_files})

# Work dir
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../")

# Test
add_test(NAME Tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../" COMMAND ${PROJECT_NAME})

# Lib/Headers
target_include_directories(${PROJECT_NAME} PUBLIC ${dptests_includes})