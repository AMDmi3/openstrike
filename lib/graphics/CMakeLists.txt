set(SOURCES
	camera.cc
	groundrenderer.cc
	objectsorter.cc
	rectpacker.cc
	renderer.cc
	spritemanager.cc
	sprites.cc
)

include_directories(${PROJECT_SOURCE_DIR}/lib ${SDL2PP_INCLUDE_DIRS})
add_library(graphics STATIC ${SOURCES})
