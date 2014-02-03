PROJECT(OpenStrike)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# common depends and definitions
ADD_DEFINITIONS("-std=c++11 -Wall -Wextra -pedantic")

# depends
ADD_SUBDIRECTORY(extlibs/SDL2pp)

# projects
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(util)
