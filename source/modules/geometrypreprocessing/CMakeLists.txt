add_library(geometryPreprocessing 
			geometrySlicing.cpp 
			geometrySlicing.h
			alignGeometry.cpp
			alignGeometry.h)
			
target_include_directories(geometryPreprocessing PUBLIC ../misc)

target_link_libraries(geometryPreprocessing PUBLIC CGAL::CGAL Boost::boost misc )
			
