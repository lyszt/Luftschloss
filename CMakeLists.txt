cmake_minimum_required(VERSION 3.16)
project(alisa-v-zadzerkalli_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# RelWithDebInfo, not Debug. Debug -O0 -g on template-heavy DPP translation
# units balloons each compiler process to several GB and OOMs the machine.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Link the prebuilt system DPP. Compiling it from source via FetchContent is
# what exhausted RAM. Install it once with `make deps`, driven by the Makefile.
find_package(dpp REQUIRED)
find_package(cpr REQUIRED)

# CONFIGURE_DEPENDS re-globs on rebuild so new src/**.cpp files get picked up
# without editing this file.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(alisa-v-zadzerkalli_core main.cpp ${SOURCES})
target_include_directories(alisa-v-zadzerkalli_core PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(alisa-v-zadzerkalli_core PRIVATE -Wall -Wextra)
target_link_libraries(alisa-v-zadzerkalli_core PRIVATE dpp::dpp cpr::cpr)
