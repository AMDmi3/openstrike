SET(SOURCES
	rectpacker.cc
	spritemanager.cc
	renderer.cc
	sprites.cc
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib ${SDL2PP_INCLUDE_DIRS})
ADD_LIBRARY(graphics STATIC ${SOURCES})
