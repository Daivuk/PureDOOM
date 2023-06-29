cmake_minimum_required(VERSION 3.0)

# Project
project(pd_sdl_example)

# add_definitions(-DDOOM_FAST_TICK)
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
file(GLOB dpsdl_src_files sdl_example.c)
file(GLOB dpsdl_doom_src_files ../../src/DOOM/*.*)
list(APPEND dpsdl_includes PUBLIC ../../src/)

# SDL
add_definitions(-DSDL_MAIN_HANDLED)
set(HAVE_LIBC ON)
add_subdirectory(../../thirdparty/SDL/ ./thirdparty/SDL/)
list(APPEND dpsdl_libs SDL2-static)
list(APPEND dpsdl_includes PUBLIC ../../thirdparty/SDL/include/)

# Source groups
source_group("Example" FILES ${dpsdl_src_files})
source_group("DOOM" FILES ${dpsdl_doom_src_files})

# Executable
add_executable(${PROJECT_NAME} ${dpsdl_src_files})

# Work dir
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../")

# For midi output on mac
if (APPLE)
    list(APPEND dpsdl_libs "-framework CoreFoundation")
    list(APPEND dpsdl_libs "-framework AudioToolbox")
    list(APPEND dpsdl_libs "-framework CoreMIDI")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
endif()

# Lib/Headers
target_include_directories(${PROJECT_NAME} PUBLIC ${dpsdl_includes})
target_link_libraries(${PROJECT_NAME} PUBLIC ${dpsdl_libs})
