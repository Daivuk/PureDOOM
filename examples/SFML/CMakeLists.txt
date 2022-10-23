cmake_minimum_required(VERSION 3.0)

# Project
project(pd_sfml_example)

set(CMAKE_CXX_STANDARD 17)
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
file(GLOB dpsfml_src_files sfml_example.cpp)
file(GLOB dpsfml_doom_src_files ../../src/DOOM/*.*)
list(APPEND dpsfml_includes PUBLIC ../../src/)

# SFML
set(BUILD_SHARED_LIBS FALSE)
set(SFML_STATIC_LIBRARIES TRUE)
add_subdirectory(../../thirdparty/SFML/ ./thirdparty/SFML/)
# if (CMAKE_BUILD_TYPE MATCHES Debug)
#     list(APPEND dpsfml_libs sfml-window-d sfml-graphics-d sfml-audio-d)
# else()
    list(APPEND dpsfml_libs sfml-window sfml-graphics sfml-audio)
# endif()
list(APPEND dpsfml_includes PUBLIC ../../thirdparty/SFML/include/)

# Source groups
source_group("Example" FILES ${dpsfml_src_files})
source_group("DOOM" FILES ${dpsfml_doom_src_files})

# Executable
add_executable(${PROJECT_NAME} ${dpsfml_src_files} ${dpsfml_doom_src_files})

# Work dir
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../")

# Copy OpenAL DLL into the working directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/SFML/extlibs/bin/x64/openal32.dll DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../)

# For midi output on mac
if (APPLE)
    list(APPEND dpsfml_libs "-framework CoreFoundation")
    list(APPEND dpsfml_libs "-framework AudioToolbox")
    list(APPEND dpsfml_libs "-framework CoreMIDI")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
endif()

# Lib/Headers
target_include_directories(${PROJECT_NAME} PUBLIC ${dpsfml_includes})
target_link_libraries(${PROJECT_NAME} PUBLIC ${dpsfml_libs})
