cmake_minimum_required(VERSION 3.1)

set(THIS_PROJECT moondemo)

project( ${THIS_PROJECT} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DAPPLE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAPPLE")

include_directories(
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/Cry
        ${CMAKE_CURRENT_SOURCE_DIR}/code/Cry/CryCommon
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/zlib-1.2.8
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/yasli
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/Libs/yasli
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/md5
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/LibTomCrypt/src/headers
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/LibTomMath
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/boost
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/lz4/lib
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/ncurses/include
#        #CryFont
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/freetype/include
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/Libs/freetype
#        #CryInput
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/SDL2/include/mac
#        #CryScriptSystem
#        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/lua/src
)

### disable all compile warnings
add_definitions(-w)

set(TOOLCHAIN_DIR "/usr/bin")
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/clang)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/clang++)

set(CMAKE_BUILD_TYPE "Shipping")
set(SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code/Libs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/Lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/Lib/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/Bin/${CMAKE_BUILD_TYPE})

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

message("Build Type:" ${CMAKE_BUILD_TYPE})
message("Build CXX Flags:" ${CMAKE_CXX_FLAGS})
message("Build C Flags:" ${CMAKE_C_FLAGS})

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
add_subdirectory("code/Libs/lua")
#add_subdirectory("code/Libs/lzss")
#add_subdirectory("code/Libs/strophe")
###error: add_subdirectory("code/Libs/tiff")

### cryengine libraries
add_subdirectory("code/Cry/CryCommon")
add_subdirectory("code/Cry/CrySystem")
#add_subdirectory("code/Cry/CryAction")
#add_subdirectory("code/Cry/CryFont")
#add_subdirectory("code/Cry/CryEntitySystem")
#add_subdirectory("code/Cry/CryInput")
#add_subdirectory("code/Cry/CryScriptSystem")

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