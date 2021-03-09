#include "computeOffset.h"

std::map<std::string, std::vector<Polygon_with_holes_ptr_vector>> computeOffset(double offset, std::map<std::string, std::vector<Polygon_2>> slicesDiffGeometryPolygon, bool saveEPSFiles, std::filesystem::path out) {
	std::filesystem::path outputPath = out;
	if (saveEPSFiles) {
		outputPath /= "epsfiles";
		std::filesystem::create_directories(outputPath);
	};
	
	std::map<std::string, std::vector<Polygon_with_holes_ptr_vector>> result;
	// Rückwärts iterieren
	for (std::map<std::string, std::vector<Polygon_2>>::reverse_iterator it = slicesDiffGeometryPolygon.rbegin(); it != slicesDiffGeometryPolygon.rend(); ++it) {
		std::cout << "------------------------------------------------------" << std::endl;
		std::cout << "Computing Offset for Slice at z= " + it->first << std::endl;
		std::vector<Polygon_2> polyvec = it->second;
		std::vector<Polygon_with_holes_ptr_vector> offsets;
		for (int i = 0; i < polyvec.size(); ++i) {
			Polygon_with_holes p_hole(polyvec[i]);
			Polygon_with_holes_ptr_vector offset_polygons = CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(offset, p_hole);
			offsets.push_back(offset_polygons);
			if (saveEPSFiles) {
				std::string filename = "_offsetAt_z_" + it->first + "(" + std::to_string(i) + ").eps";
				std::ofstream eps(outputPath / filename);
				if (eps)
				{
					dump_to_eps(p_hole, offset_polygons, eps);
				}
				else
				{
					std::cerr << "Could not open result file: " << filename << std::endl;
				}
			}

		}
		result.insert(std::pair<std::string, std::vector<Polygon_with_holes_ptr_vector>>(it->first, offsets));
	};

	return result;
};