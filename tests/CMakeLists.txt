INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
ADD_EXECUTABLE(test_geom test_geom.cc)
ADD_TEST(test_geom test_geom)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
ADD_EXECUTABLE(test_bbox test_bbox.cc)
ADD_TEST(test_bbox test_bbox)
