add_library(rawMaterial
			rawMaterial.cpp
			rawMaterial.h
			rawMaterialLoader.cpp
			rawMaterialLoader.h 
			rawMaterialSelection.cpp 
			rawMaterialSelection.h
			rawMaterialLoader.cpp
			rawMaterialLoader.h)

target_include_directories(rawMaterial PUBLIC ../io ../geometrypreprocessing)


target_link_libraries(rawMaterial PUBLIC CGAL::CGAL IO geometryPreprocessing nlohmann_json)
			
