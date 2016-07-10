# sublime text cmake plugin
# cocos cmake learning
# cmake qt
# variable case sensitive
# cmake function usage
# cmake macro usage()
# function message print function
# function set
# variable CMAKE_MODULE_PATH

# function FILE GLOB
# function add_library
# function add_executable
# function add_subdirectory
# function add_definitions
# function add_dependencies

cmake_minimum_required(VERSION 3.1)

project(pycmake)

# define some variables
set(PYCM_PROJECT_VERSION 0.0.2)
set(PYCM_PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PYCM_PROJECT_EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules2/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules3/")

# PYCM_BUILD_TYPE has precedence over DEBUG_MODE
# Still supporting DEBUG_MODE for backwards compatibility
if (NOT PYCM_BUILD_TYPE)
  if(DEBUG_MODE)
    set(PYCM_BUILD_TYPE DEBUG)
  else(DEBUG_MODE)
    set(PYCM_BUILD_TYPE RELEASE)
  endif(DEBUG_MODE)
endif(NOT PYCM_BUILD_TYPE)

include(CocosBuildHelpers)

message(${BUILDING_STRING})

function(pycm_func_log param)
    message("====pycm function call: [" ${param} "]====")
endfunction(pycm_func_log)

function(pycm_cmake_version_cook)
    pycm_func_log("pycm_cmake_version_cook")
    # Use new behavior with cmake > 3.1:
    # Only interpret if() arguments as variables or keywords when unquoted
    if(CMAKE_VERSION VERSION_GREATER 3.1)
        cmake_policy(SET CMP0054 NEW)
    endif()
endfunction(pycm_cmake_version_cook)

function(pycm_cmake_policy_cook)
    pycm_func_log("pycm_cmake_policy_cook")
    # It ensures that when Find*.cmake files included from cmake's Modules dir
    # include another *.cmake file with relative path, that file will be included
    # also from cmake's Modules dir, to not clash with per-project files.
    cmake_policy(SET CMP0017 NEW)
endfunction(pycm_cmake_policy_cook)

function(pycm_cmake_print_info)
    pycm_func_log("pycm_cmake_print_info")
    message("PROJECT_NAME: " ${PROJECT_NAME})
    message("PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR})
    message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
    # message("PROJECT_LABEL: " ${PROJECT_LABEL})
    message("CMAKE_VERSION: " ${CMAKE_VERSION})
    message("CMAKE_MAJOR_VERSION: " ${CMAKE_MAJOR_VERSION})
    message("CMAKE_MINOR_VERSION: " ${CMAKE_MINOR_VERSION})
    message("CMAKE_SYSTEM_VERSION: " ${CMAKE_SYSTEM_VERSION})
    message("CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

    message("PYCM_PROJECT_VERSION: " ${PYCM_PROJECT_VERSION})
    message("PYCM_BUILD_TYPE: " ${PYCM_BUILD_TYPE})
    message("PYCM_SYSTEM_STRING:" "${PYCM_SYSTEM_STRING}")
endfunction(pycm_cmake_print_info)

function(pycm_cmake_project_registry)
    pycm_func_log("pycm_cmake_project_registry")
    message("PYCM_PROJECT_VERSION: " ${PYCM_PROJECT_VERSION})
    message("PYCM_PROJECT_ROOT_DIR: " ${PYCM_PROJECT_ROOT_DIR})
    message("PYCM_PROJECT_EXTERNAL_DIR: " ${PYCM_PROJECT_EXTERNAL_DIR})
endfunction(pycm_cmake_project_registry)

function(pycm_cmake_architecture_info)
    pycm_func_log("pycm_cmake_architecture_info")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PYCM_ARCH_STR "64-bit")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(PYCM_ARCH_STR "32-bit")
    else()
        message(FATAL_ERROR "Unsupported architecture, CMake will exit")
        return()
    endif()
    message("PYCM_ARCH_STR: " ${PYCM_ARCH_STR})
endfunction(pycm_cmake_architecture_info)

function(pycm_cmake_platform_info)
     foreach(var_ ${OSX_ARCHITECTURES})
        message(${var_})
     endforeach()
endfunction(pycm_cmake_platform_info)

function(pycm_cmake_module_path_info)
    pycm_func_log("pycm_cmake_module_path_info")
    foreach(var_ ${CMAKE_MODULE_PATH})
        message("cmake module path: " ${var_})
    endforeach()
endfunction(pycm_cmake_module_path_info)

macro(pycm_macro_registry)
    set(PYCM_PROJECT_VERSION 0.04)
endmacro(pycm_macro_registry)

pycm_cmake_version_cook()
pycm_cmake_policy_cook()
pycm_macro_registry()
pycm_cmake_project_registry()
pycm_cmake_print_info()
pycm_cmake_architecture_info()
pycm_cmake_platform_info()
pycm_cmake_module_path_info()
