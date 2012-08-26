PROJECT (TerrainTest)

INCLUDE_DIRECTORIES( ../../../include/ ../../asnCommon/ )

#SET(CMAKE_VERBOSE_MAKEFILE "ON")

SET(CMAKE_CXX_FLAGS "-Os" )
ADD_DEFINITIONS( -fPIC)

SET(EXECUTABLE_OUTPUT_PATH "../../../bin/binl" )

SET ( UTIL_DIR ../../../src/asnCommon/ASN_utils.cpp )
AUX_SOURCE_DIRECTORY ( . MAIN_DIR)
AUX_SOURCE_DIRECTORY (../../../include INC_DIR)

ADD_EXECUTABLE (TerrainTest ${MAIN_DIR} ${UTIL_DIR} ${INC_DIR})

#TARGET_LINK_LIBRARIES(TerrainTest -ldl)

TARGET_LINK_LIBRARIES(TerrainTest -ldl -lSDL)
