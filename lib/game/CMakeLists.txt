SET(SOURCES
	game.cc
	gameobject.cc
	levelloader.cc
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
ADD_LIBRARY(game STATIC ${SOURCES})
