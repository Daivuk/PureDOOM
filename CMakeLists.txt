cmake_minimum_required(VERSION 3.0)

# Project
project(PureDOOM)

# Defines
if (UNIX AND NOT APPLE)
    set(LINUX 1)
endif()

if (LINUX)
    add_definitions(-DDDSP_LINUX)
endif()

if (WIN32)
    add_definitions(-DDDSP_WIN32)
endif()

if (APPLE)
    add_definitions(-DDDSP_OSX)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)
endif()

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
file(GLOB ddsp_src_files src/*.*)
file(GLOB ddsp_doom_src_files src/DOOM/*.*)

# SDL
add_definitions(-DSDL_MAIN_HANDLED)
set(HAVE_LIBC ON)
add_subdirectory(./thirdparty/SDL/)
list(APPEND ddsp_libs SDL2-static)
list(APPEND ddsp_includes PUBLIC ./thirdparty/SDL/include/)

# Source groups
source_group("Example" FILES ${ddsp_src_files})
source_group("DOOM" FILES ${ddsp_doom_src_files})

# Executable
add_executable(${PROJECT_NAME} ${ddsp_src_files} ${ddsp_doom_src_files})

# Work dir
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

# For midi output on mac
if (APPLE)
    list(APPEND ddsp_libs "-framework CoreFoundation")
    list(APPEND ddsp_libs "-framework AudioToolbox")
    list(APPEND ddsp_libs "-framework CoreMIDI")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
endif()

# Lib/Headers
target_include_directories(${PROJECT_NAME} PUBLIC ${ddsp_includes})
target_link_libraries(${PROJECT_NAME} PUBLIC ${ddsp_libs})
