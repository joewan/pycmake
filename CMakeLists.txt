cmake_minimum_required(VERSION 3.1)

project(pycmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DAPPLE")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/code/Cry
        ${CMAKE_CURRENT_SOURCE_DIR}/code/Cry/CryCommon
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/zlib-1.2.8
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/yasli
        ${CMAKE_CURRENT_SOURCE_DIR}/code/Libs/yasli
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/md5
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/LibTomCrypt/src/headers
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/LibTomMath
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/boost
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/lz4/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/ncurses/include
        #CryFont
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/freetype/include
        #CryInput
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/SDL2/include/mac
        #CryScriptSystem
        ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs/lua/src
)

### disable all compile warnings
add_definitions(-w)

set(TOOLCHAIN_DIR "/usr/bin")
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/clang)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/clang++)

set(CMAKE_BUILD_TYPE "Shipping")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/Lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/Lib/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/Bin/${CMAKE_BUILD_TYPE})

message("Build Type:" ${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS})

add_subdirectory("code/Cry/CryCommon")
#add_subdirectory("code/Cry/CrySystem")
#add_subdirectory("code/Cry/CryAction")
#add_subdirectory("code/Cry/CryFont")
#add_subdirectory("code/Cry/CryEntitySystem")
add_subdirectory("code/Cry/CryInput")
add_subdirectory("code/Cry/CryScriptSystem")

set(SOURCE_FILES main.cpp)
add_executable(pycmake ${SOURCE_FILES})
