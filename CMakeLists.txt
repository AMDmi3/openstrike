PROJECT(OpenStrike)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# common depends and definitions
ADD_DEFINITIONS("-std=c++11 -Wall -Wextra -pedantic")
ADD_DEFINITIONS("-DDEBUG_RENDERING")

# depends
IF(NOT EXISTS ${PROJECT_SOURCE_DIR}/extlibs/SDL2pp/CMakeLists.txt)
    MESSAGE(FATAL_ERROR "The source directory\n  ${PROJECT_SOURCE_DIR}/extlibs/SDL2pp\ndoes not contain a CMakeLists.txt file.\nIt is likely that you forgot to run\n  git submodule init && git submodule update")
ENDIF(NOT EXISTS ${PROJECT_SOURCE_DIR}/extlibs/SDL2pp/CMakeLists.txt)
ADD_SUBDIRECTORY(extlibs/SDL2pp)

# projects
ENABLE_TESTING()

ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)
