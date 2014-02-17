SET(SOURCES
	bullet.cc
	explosion.cc
	heli.cc
	hellfire.cc
	hydra.cc
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
ADD_LIBRARY(gameobjects STATIC ${SOURCES})
