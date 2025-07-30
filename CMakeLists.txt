cmake_minimum_required(VERSION 3.26)

project(connect-4)

include(cmake/Ipo.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/DeployQt.cmake)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No setting build type was specified. Setting Release build")
  set(CMAKE_BUILD_TYPE Build)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generate compile_commands.json to make it easier to work with clang based tools

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(BUILD_TESTING "Build tests for project" True)
option(BUILD_GUI "Build connect4 gui with gt6" False)

add_subdirectory(src)

if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(tests)
endif()
