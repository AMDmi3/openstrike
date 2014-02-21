SET(SOURCES
	main.cc
)

SET(STATIC_LIBRARIES
	graphics
	gameobjects
	game
	dat
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib ${SDL2PP_INCLUDE_DIRS})
ADD_EXECUTABLE(openstrike ${SOURCES})
# static libraries are included twice to solve cyclic depends
TARGET_LINK_LIBRARIES(openstrike ${STATIC_LIBRARIES} ${STATIC_LIBRARIES} ${SDL2PP_LIBRARIES})
