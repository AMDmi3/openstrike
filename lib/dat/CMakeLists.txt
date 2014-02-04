SET(SOURCES
	buffer.cc
	datfile.cc
	datgraphics.cc
	unpacker.cc
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/extlibs/boost)
ADD_LIBRARY(datfile STATIC ${SOURCES})
