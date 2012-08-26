cmake_minimum_required(VERSION 0.0)

PROJECT (AmberSkyNetVR)

INCLUDE_DIRECTORIES( ../../include/ ../../src/asnCommon/ ../../src/asnDeviceGL/lib/ 
../../src/asnNetIRC  ../../src/asnScriptManager/lib/lua/include/ /usr/include/freetype2 )

#SET(CMAKE_VERBOSE_MAKEFILE "ON")

#SET(CMAKE_CXX_FLAGS "-Wno-deprecated" )
SET(CMAKE_CXX_FLAGS "-Os" )

#-static" )

ADD_DEFINITIONS( -fPIC)

SET(EXECUTABLE_OUTPUT_PATH "../../bin/binl" )
SET(LIBRARY_OUTPUT_PATH "../../bin/binl/plugins" )

AUX_SOURCE_DIRECTORY (../../include INC_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnMain MAIN_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnEngine ENGINE_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnCommon COMMON_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnWorld WORLD_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnNodeSprite SPRITE_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnNodeGUI_Simple SGUI_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnNetSupervisor NET_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnNetIRC NETIRC_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnNetJabber NETJABBER_DIR)


AUX_SOURCE_DIRECTORY (../../src/asnNodeHeightMap HEIGHTMAP_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnEmptyPlugin EMPTY_DIR)

AUX_SOURCE_DIRECTORY (../../src/asnNetHTTP HTTP_DIR)

AUX_SOURCE_DIRECTORY (../../src/asnNodeSLTerrain SLTERRAIN_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnNodeRuler RULER_DIR)

AUX_SOURCE_DIRECTORY (../../src/asnScriptManager SCRIPT_DIR)
AUX_SOURCE_DIRECTORY (../../src/asnScriptManager/lib/lua/src SCRIPT_DIR_ADD)
AUX_SOURCE_DIRECTORY (../../src/asnScriptManager/lib/lua/src/lib SCRIPT_DIR_ADD2)

AUX_SOURCE_DIRECTORY (../../src/asnEngine/asnDataParser PARSER_DIR)
SET ( PARSER_ADD 
../../src/asnEngine/asnDataParser/lib/tinyXML/tinyxml.cpp 
../../src/asnEngine/asnDataParser/lib/tinyXML/tinystr.cpp 
../../src/asnEngine/asnDataParser/lib/tinyXML/tinyxmlerror.cpp 
../../src/asnEngine/asnDataParser/lib/tinyXML/tinyxmlparser.cpp 
)

AUX_SOURCE_DIRECTORY (../../src/asnEngine/asnLog LOG_DIR)

AUX_SOURCE_DIRECTORY (../../src/asnEngine/asnFileSystem FILESYS_DIR)

AUX_SOURCE_DIRECTORY (../../src/asnFileZIP FILESYSZIP_DIR)

SET ( FILESYSZIP_ADD 
../../src/asnFileZIP/lib/zlib/inflate.c 
../../src/asnFileZIP/lib/zlib/crc32.c 
../../src/asnFileZIP/lib/zlib/adler32.c 
../../src/asnFileZIP/lib/zlib/inftrees.c
../../src/asnFileZIP/lib/zlib/inffast.c
../../src/asnFileZIP/lib/zlib/zutil.c
../../src/asnFileZIP/lib/minizip/unzip.c
../../src/asnFileZIP/lib/minizip/ioapi.c
)


#SOURCE_FILES_REMOVE( LOG_DIR ../src/asnLog/asnLog.cpp)

AUX_SOURCE_DIRECTORY (../../src/asnSound SOUND_DIR)

AUX_SOURCE_DIRECTORY (../../src/asnNode3ds NODE3DS_DIR)
SET (NODE3DS_ADD ../../src/asnNode3ds/lib/l3ds.cpp)

AUX_SOURCE_DIRECTORY (../../src/asnDeviceGL DEVICE_DIR)
SET (DEVICE_ADD ../../src/asnDeviceGL/lib/GLee.c)

#MESSAGE ( ${FILESYS_DIR} )

#ADD_CUSTOM_COMMAND( TARGET AmberSkyNetVR
# rm -rf Makefile)

#ADD_CUSTOM_COMMAND(
#TARGET mylib
#POST_BUILD
#COMMAND copy ARGS ${MYLIBRARY_SOURCE_DIR}/lib/release/mylib.dll
#${MYLIBRARY_SOURCE_DIR}/bin/release
#COMMAND copy ARGS ${MYLIBRARY_SOURCE_DIR}/lib/release/mylib.lib
#${MYLIBRARY_SOURCE_DIR}/lib
#)

ADD_EXECUTABLE (AmberSkyNet ${MAIN_DIR} ${INC_DIR} )
ADD_LIBRARY (AmberSkyNetVR SHARED ${ENGINE_DIR} ${COMMON_DIR} ${INC_DIR} ${PARSER_DIR} ${PARSER_ADD} ${LOG_DIR} ${FILESYS_DIR} )

#ADD_CUSTOM_TARGET( confclean COMMAND rm -rf Makefile CMakeFiles/ CMakeCache.txt cmake_install.cmake DartTestfile.txt install_manifest.txt )

#${FILESYS_DIR}
#${FS1_DIR} ${FS2_DIR} ${LOG_DIR} )
#ADD_LIBRARY (asnLog SHARED ${LOG_DIR} ${COMMON_DIR} ${INC_DIR} )

ADD_LIBRARY (asnFileZIP SHARED ${FILESYSZIP_DIR} ${FILESYSZIP_ADD} ${COMMON_DIR} ${INC_DIR} )

#ADD_LIBRARY (asnDataParser SHARED ${PARSER_DIR} ${PARSER_ADD} ${COMMON_DIR} ${INC_DIR}  )

ADD_LIBRARY (asnWorld SHARED ${WORLD_DIR} ${COMMON_DIR} ${INC_DIR} )

ADD_LIBRARY (asnNodeSprite SHARED ${SPRITE_DIR} ${COMMON_DIR} ${INC_DIR} )
ADD_LIBRARY (asnNodeSLTerrain SHARED ${SLTERRAIN_DIR} ${COMMON_DIR} ${INC_DIR} )
ADD_LIBRARY (asnNodeRuler SHARED ${RULER_DIR} ${COMMON_DIR} ${INC_DIR} )

ADD_LIBRARY (asnNode3ds SHARED ${NODE3DS_DIR} ${COMMON_DIR} ${NODE3DS_ADD} ${INC_DIR} )

ADD_LIBRARY (asnNodeHeightMap SHARED ${HEIGHTMAP_DIR} ${COMMON_DIR} ${INC_DIR} )

#ADD_LIBRARY (asnEmptyPlugin SHARED ${EMPTY_DIR} ${COMMON_DIR} ${INC_DIR} )

ADD_LIBRARY (asnNodeGUI_Simple SHARED ${SGUI_DIR} ${COMMON_DIR} ${INC_DIR} )

ADD_LIBRARY (asnSound SHARED ${SOUND_DIR} ${COMMON_DIR} ${INC_DIR} )
ADD_LIBRARY (asnNetSupervisor SHARED ${NET_DIR} ${COMMON_DIR} ${INC_DIR} )
#ADD_LIBRARY (asnNetIRC SHARED ${NETIRC_DIR} ${COMMON_DIR} ${INC_DIR} )
ADD_LIBRARY (asnNetHTTP SHARED ${HTTP_DIR} ${COMMON_DIR} ${INC_DIR} )

ADD_LIBRARY (asnNetJabber SHARED ${NETJABBER_DIR} ${COMMON_DIR} ${INC_DIR} )

ADD_LIBRARY (asnDeviceGl SHARED ${DEVICE_DIR} ${DEVICE_ADD} ${COMMON_DIR} ${INC_DIR} )

ADD_LIBRARY (asnScriptManager SHARED ${SCRIPT_DIR} ${COMMON_DIR} ${INC_DIR} ${SCRIPT_DIR_ADD} ${SCRIPT_DIR_ADD2} )

TARGET_LINK_LIBRARIES(AmberSkyNetVR -ldl -lSDL)

TARGET_LINK_LIBRARIES(AmberSkyNet -ldl)

TARGET_LINK_LIBRARIES(asnDeviceGl -lGL -lGLU -lSDL -lSDL_image -lfreetype)

TARGET_LINK_LIBRARIES(asnSound -lSDL_mixer)
TARGET_LINK_LIBRARIES(asnNetSupervisor -lSDL_net)

#ADD_CUSTOM_COMMAND(
#TARGET AmberSkyNetVR
#DEPENDS libAmberSkyNetVR.so
#WORKING_DIRECTORY ../../bin/binl/
#COMMAND ln -s plugins/libAmberSkyNetVR.so libAmberSkyNetVR.so
#)
