set(SOURCES
	unpacker.cc
)

include_directories(${PROJECT_SOURCE_DIR}/lib)
add_executable(unpacker ${SOURCES})
target_link_libraries(unpacker dat)
