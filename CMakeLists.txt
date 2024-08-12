cmake_minimum_required(VERSION 3.22)

project(OpenStrike LANGUAGES CXX VERSION 0.0.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_definitions("-Wall -Wextra -pedantic")
add_definitions("-DDEBUG_RENDERING")

# depends
if(NOT EXISTS ${PROJECT_SOURCE_DIR}/extlibs/SDL2pp/CMakeLists.txt)
    message(FATAL_ERROR "The source directory\n  ${PROJECT_SOURCE_DIR}/extlibs/SDL2pp\ndoes not contain a CMakeLists.txt file.\nIt is likely that you forgot to run\n  git submodule init && git submodule update")
endif()
add_subdirectory(extlibs/SDL2pp)

# projects
enable_testing()

add_subdirectory(lib)
add_subdirectory(util)
add_subdirectory(src)
add_subdirectory(tests)
