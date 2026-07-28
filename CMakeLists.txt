cmake_minimum_required(VERSION 3.16)
project(wunderland_core LANGUAGES CXX)

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

add_executable(wunderland_core main.cpp)
target_compile_options(wunderland_core PRIVATE -Wall -Wextra)
target_link_libraries(wunderland_core PRIVATE dpp::dpp)
