set(SOURCES
	game.cc
	gameobject.cc
	levelloader.cc
)

include_directories(${PROJECT_SOURCE_DIR}/lib)
add_library(game STATIC ${SOURCES})
