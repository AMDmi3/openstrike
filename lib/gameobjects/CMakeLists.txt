set(SOURCES
	building.cc
	explosion.cc
	heli.cc
	projectile.cc
	unit.cc
)

include_directories(${PROJECT_SOURCE_DIR}/lib)
add_library(gameobjects STATIC ${SOURCES})
