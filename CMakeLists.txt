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

find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)
include(CGAL_Eigen_support)


find_package(nlohmann_json REQUIRED) #JSON-Lib: https://github.com/nlohmann/json


#Module
add_subdirectory(source/modules)

#create_single_source_cgal_program( "main.cpp")
add_executable(Program source/main.cpp)
target_link_libraries(Program PUBLIC CGAL::CGAL nlohmann_json misc IO geometryPreprocessing rawMaterial millingmachine)
add_custom_command(TARGET Program POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:Program>)
					   
					   
