set(SOURCES
	gfxviewer.cc
)

include_directories(${PROJECT_SOURCE_DIR}/lib ${SDL2PP_INCLUDE_DIRS})
add_executable(gfxviewer ${SOURCES})
target_link_libraries(gfxviewer dat ${SDL2PP_LIBRARIES})
