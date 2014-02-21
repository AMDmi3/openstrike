SET(SOURCES
	building.cc
	bullet.cc
	explosion.cc
	heli.cc
	rocket.cc
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
ADD_LIBRARY(gameobjects STATIC ${SOURCES})
