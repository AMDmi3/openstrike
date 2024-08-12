include_directories(${PROJECT_SOURCE_DIR}/lib)
add_executable(test_geom test_geom.cc)
add_test(test_geom test_geom)

include_directories(${PROJECT_SOURCE_DIR}/lib)
add_executable(test_bbox test_bbox.cc)
add_test(test_bbox test_bbox)
