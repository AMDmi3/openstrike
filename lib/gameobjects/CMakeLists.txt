SET(SOURCES
	building.cc
	explosion.cc
	heli.cc
	projectile.cc
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
ADD_LIBRARY(gameobjects STATIC ${SOURCES})
