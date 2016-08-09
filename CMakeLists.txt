cmake_minimum_required(VERSION 3.1)

set(THIS_PROJECT MoonEngine)
project( ${THIS_PROJECT} )
message("PROJECT " ${THIS_PROJECT} "|" ${CMAKE_CURRENT_LIST_DIR})

function(moon_cmake_project_registry)
    set(ENV{MOON_PROJECT_VERSION} "0.01")
endfunction(moon_cmake_project_registry)

moon_cmake_project_registry()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DAPPLE")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAPPLE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN64")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWIN64")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/code/Cry
        ${CMAKE_CURRENT_SOURCE_DIR}/code/Cry/CryCommon
)

### disable all compile warnings
###-DNOT_USE_CRY_STRING
add_definitions(-w -DXML_STATIC -DNOT_USE_CRY_MEMORY_MANAGER -DCRY_PLATFORM_WINAPI)

set(TOOLCHAIN_DIR "/usr/bin")
#set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/clang)
#set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/clang++)

set(CMAKE_BUILD_TYPE "Shipping")
set(SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code/SDKs)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code/Libs)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/Lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/Lib/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/Bin/${CMAKE_BUILD_TYPE})

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
#add_subdirectory("code/Libs/lua")
###add_subdirectory("code/Libs/lzss")
###add_subdirectory("code/Libs/strophe")
###error: add_subdirectory("code/Libs/tiff")

### cryengine libraries
add_subdirectory("code/Cry/CryCommon")
add_subdirectory("code/Cry/CrySystem")
#add_subdirectory("code/Cry/CryAction")
#add_subdirectory("code/Cry/CryFont")
#add_subdirectory("code/Cry/CryEntitySystem")
#add_subdirectory("code/Cry/CryInput")
#add_subdirectory("code/Cry/CryScriptSystem")

### demo
add_subdirectory("code/Demo")
