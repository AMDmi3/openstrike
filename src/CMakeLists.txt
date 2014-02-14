SET(SOURCES
	main.cc
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib ${SDL2PP_INCLUDE_DIRS})
ADD_EXECUTABLE(openstrike ${SOURCES})
# take care of library order for correct static linking
TARGET_LINK_LIBRARIES(openstrike graphics gameobjects game dat ${SDL2PP_LIBRARIES})
