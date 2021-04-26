add_library(misc 
			enums.cpp 
			enums.h
			helpFunctions.cpp
			helpFunctions.h)
			
target_link_libraries(misc PUBLIC CGAL::CGAL)
			
