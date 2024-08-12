set(SOURCES
	fonttest.cc
)

include_directories(${PROJECT_SOURCE_DIR}/lib ${SDL2PP_INCLUDE_DIRS})
add_executable(fonttest ${SOURCES})
target_link_libraries(fonttest graphics dat ${SDL2PP_LIBRARIES})
