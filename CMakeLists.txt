#@+leo-ver=5-thin
#@+node:gcross.20101121162317.1588: * @thin CMakeLists.txt
cmake_minimum_required (VERSION 2.6)
project (unit--)

if( NOT CMAKE_BUILD_TYPE )
set( CMAKE_BUILD_TYPE "RelWithDebInfo" )
endif ()

add_subdirectory ("include")
add_subdirectory ("src")
add_subdirectory ("tests")
#@-leo
