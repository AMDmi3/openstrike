SET(SOURCES
	mapviewer.cc
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib ${SDL2PP_INCLUDE_DIRS})
ADD_EXECUTABLE(mapviewer ${SOURCES})
TARGET_LINK_LIBRARIES(mapviewer dat game graphics gameobjects dat game graphics gameobjects ${SDL2PP_LIBRARIES})
