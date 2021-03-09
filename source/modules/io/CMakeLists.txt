add_library(IO 
			fileio.cpp 
			fileio.h
			machineloader.cpp
			machineloader.h
			dump_to_eps.h
			config.cpp
			config.h)

target_include_directories(IO PUBLIC ../millingmachine)

			
target_link_libraries(IO PUBLIC CGAL::CGAL millingmachine)			
