# sublime text cmake plugin
# cocos cmake learning
# cmake function usage
# function message print function
# function set

cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0017 NEW)

project(pycmake)

set(PYCMAKE_VERSION 0.0.1)

function(pycm_func_log param)
    message("====pycm function call: [" ${param} "]====")
endfunction(pycm_func_log)

function(pycm_cmake_version_cook)
    pycm_func_log("pycm_cmake_version_cook")
    # Use new behavior with cmake >= 3.1:
    # Only interpret if() arguments as variables or keywords when unquoted
endfunction(pycm_cmake_version_cook)

function(pycm_cmake_policy_cook)
    pycm_func_log("pycm_cmake_policy_cook")
endfunction(pycm_cmake_policy_cook)

function(pycm_cmake_print_info)
    pycm_func_log("pycm_cmake_print_info")

    message("PYCMAKE_VERSION: " ${PYCMAKE_VERSION})
    message("PROJECT_NAME: " ${PROJECT_NAME})
    message("PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR})
    message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
    # message("PROJECT_LABEL: " ${PROJECT_LABEL})
    message("CMAKE_VERSION: " ${CMAKE_VERSION})
    message("CMAKE_MAJOR_VERSION: " ${CMAKE_MAJOR_VERSION})
    message("CMAKE_MINOR_VERSION: " ${CMAKE_MINOR_VERSION})
    message("CMAKE_SYSTEM_VERSION: " ${CMAKE_SYSTEM_VERSION})
endfunction(pycm_cmake_print_info)

pycm_cmake_version_cook()
pycm_cmake_policy_cook()
pycm_cmake_print_info()
