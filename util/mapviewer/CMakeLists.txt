set(SOURCES
	mapviewer.cc
)

include_directories(${PROJECT_SOURCE_DIR}/lib ${SDL2PP_INCLUDE_DIRS})
add_executable(mapviewer ${SOURCES})
target_link_libraries(mapviewer dat game graphics gameobjects dat game graphics gameobjects ${SDL2PP_LIBRARIES})
