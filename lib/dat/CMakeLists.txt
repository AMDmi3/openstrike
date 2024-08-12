set(SOURCES
	buffer.cc
	datfile.cc
	datgraphics.cc
	datlevel.cc
	unpacker.cc
)

include_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/extlibs/boost)
add_library(dat STATIC ${SOURCES})
