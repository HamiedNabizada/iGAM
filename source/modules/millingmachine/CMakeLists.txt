add_library(millingmachine
			millingMachine.cpp
			millingMachine.h
			millingTool.cpp
			millingTool.h 
			positioningMillingTool.cpp 
			positioningMillingTool.h
			computeOffset.cpp
			computeOffset.h
			removableVolume.cpp
			removableVolume.h)

target_include_directories(millingmachine PUBLIC ../io ../geometrypreprocessing ../rawMaterial)


target_link_libraries(millingmachine PUBLIC CGAL::CGAL IO geometryPreprocessing nlohmann_json rawMaterial)
			
