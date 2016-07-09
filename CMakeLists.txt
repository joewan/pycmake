#sublime text cmake plugin
#cocos cmake learning
#cmake function usage
#function message print function
#function set

cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0017 NEW)

project(pycmake)

set(PYCMAKE_VERSION 3.1.2)

function(pycm_cmake_print)
    message("ORYOL_VERSION: " ${ORYOL_VERSION})
    message("PROJECT_NAME: " ${PROJECT_NAME})
    message("PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR})
    message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
    message("PROJECT_LABEL: " ${PROJECT_LABEL})
    message("CMAKE_VERSION: " ${CMAKE_VERSION})
    message("CMAKE_MAJOR_VERSION: " ${CMAKE_MAJOR_VERSION})
    message("CMAKE_MINOR_VERSION: " ${CMAKE_MINOR_VERSION})
    message("CMAKE_SYSTEM_VERSION: " ${CMAKE_SYSTEM_VERSION})
endfunction(oryol_cmake_print)

pycm_cmake_print()
