cmake_minimum_required(VERSION 3.1...3.15)
set(VCPKG_FEATURE_FLAGS "versions,manifests")
project( igma )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(MSVC)
	message(Status " MSVC detected.")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

# CGAL-Library
find_package( CGAL REQUIRED COMPONENTS Core)
if ( NOT CGAL_FOUND )
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()
endif()

find_package ( GMP )
if (NOT GMP_FOUND)
	message(STATUS "GMP NOT FOUND")
endif()

find_package(nlohmann_json REQUIRED) #JSON-Lib: https://github.com/nlohmann/json

#Module
add_subdirectory(source/modules)

#create_single_source_cgal_program( "main.cpp")
add_executable(igma source/main.cpp)
target_link_libraries(igma PUBLIC CGAL::CGAL nlohmann_json IO geometryPreprocessing rawMaterial millingmachine)
