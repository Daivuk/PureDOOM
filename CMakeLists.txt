cmake_minimum_required(VERSION 3.0)

project(PureDOOM)

option(PUREDOOM_BUILD_SDL_EXAMPLE "" OFF)
option(PUREDOOM_BUILD_TESTS "" OFF)

if (PUREDOOM_BUILD_SDL_EXAMPLE)
    add_subdirectory(examples/SDL)
endif()
if (PUREDOOM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(examples/Tests)
endif()
