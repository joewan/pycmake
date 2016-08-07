cmake_minimum_required(VERSION 3.1)

set(THIS_PROJECT MoonDemo)

project( ${THIS_PROJECT} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DAPPLE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAPPLE")

include_directories(
        ${PROJECT_ROOT}/code/Cry
        ${PROJECT_ROOT}/code/Cry/CryCommon
        ${PROJECT_ROOT}/code/SDKs/zlib-1.2.8
        ${PROJECT_ROOT}/code/SDKs/yasli
        ${PROJECT_ROOT}/code/Libs/yasli
        ${PROJECT_ROOT}/code/SDKs/md5
        ${PROJECT_ROOT}/code/SDKs
        ${PROJECT_ROOT}/code/SDKs/LibTomCrypt/src/headers
        ${PROJECT_ROOT}/code/SDKs/LibTomMath
        ${PROJECT_ROOT}/code/SDKs/boost
        ${PROJECT_ROOT}/code/SDKs/lz4/lib
        ${PROJECT_ROOT}/code/SDKs/ncurses/include
#        #CryFont
#        ${PROJECT_ROOT}/code/SDKs/freetype/include
#        ${PROJECT_ROOT}/code/Libs/freetype
#        #CryInput
#        ${PROJECT_ROOT}/code/SDKs/SDL2/include/mac
        #CryScriptSystem && Lua
        ${PROJECT_ROOT}/code/SDKs/lua/src
)

### disable all compile warnings
### add_definitions(-w)
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

### external libraries
#add_subdirectory("code/Libs/zlib")
#add_subdirectory("code/Libs/yasli")
#add_subdirectory("code/Libs/md5")
#add_subdirectory("code/Libs/lz4")
#add_subdirectory("code/Libs/lzma")
#add_subdirectory("code/Libs/freetype")
#add_subdirectory("code/Libs/expat")
#add_subdirectory("code/Libs/tomcrypt")
#add_subdirectory("code/Libs/bigdigits")
#add_subdirectory("${PROJECT_ROOT}/code/Libs/lua")
#add_subdirectory("code/Libs/lzss")
#add_subdirectory("code/Libs/strophe")
###error: add_subdirectory("code/Libs/tiff")

### cryengine libraries
#add_subdirectory("${PROJECT_ROOT}/code/Cry/CryCommon")
#add_subdirectory("${PROJECT_ROOT}/code/Cry/CrySystem")
#add_subdirectory("code/Cry/CryAction")
#add_subdirectory("code/Cry/CryFont")
#add_subdirectory("code/Cry/CryEntitySystem")
#add_subdirectory("code/Cry/CryInput")
#add_subdirectory("code/Cry/CryScriptSystem")

add_definitions("-framework CoreServices -framework Security")

set(SOURCE_FILES main.cpp)
add_executable(${THIS_PROJECT} ${SOURCE_FILES})

target_link_libraries(${THIS_PROJECT} CrySystem)
#target_link_libraries(${THIS_PROJECT} tomcrypt )
#target_link_libraries(${THIS_PROJECT} expat )
#target_link_libraries(${THIS_PROJECT} md5)
#target_link_libraries(${THIS_PROJECT} yasli)
target_link_libraries(${THIS_PROJECT} lua)
#target_link_libraries(${THIS_PROJECT} lz4)
#target_link_libraries(${THIS_PROJECT} zlib)
