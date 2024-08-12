set(SOURCES
	main.cc
)

set(STATIC_LIBRARIES
	graphics
	gameobjects
	game
	dat
)

include_directories(${PROJECT_SOURCE_DIR}/lib ${SDL2PP_INCLUDE_DIRS})
add_executable(openstrike ${SOURCES})
# static libraries are included twice to solve cyclic depends
target_link_libraries(openstrike ${STATIC_LIBRARIES} ${STATIC_LIBRARIES} ${SDL2PP_LIBRARIES})
