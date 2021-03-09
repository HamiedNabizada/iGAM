add_library(geometryPreprocessing 
			geometrySlicing.cpp 
			geometrySlicing.h
			alignGeometry.cpp
			alignGeometry.h)
			
target_link_libraries(geometryPreprocessing PUBLIC CGAL::CGAL)
			
