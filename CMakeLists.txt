cmake_minimum_required(VERSION 3.1)

project(pycmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/code/Cry
        ${CMAKE_CURRENT_SOURCE_DIR}/code/Cry/CryCommon
)

add_subdirectory("code/Cry/CryCommon")
add_subdirectory("code/Cry/CrySystem")
add_subdirectory("code/Cry/CryFont")
add_subdirectory("code/Cry/CryEntitySystem")
add_subdirectory("code/Cry/CryInput")
add_subdirectory("code/Cry/CryScriptSystem")

#target_link_libraries(pycmake CrySystem)

set(SOURCE_FILES main.cpp)
add_executable(pycmake ${SOURCE_FILES})
