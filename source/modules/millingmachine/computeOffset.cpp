#include "computeOffset.h"

std::map<std::string, std::vector<Polygon_with_holes_ptr_vector>> computeOffset(double offset, std::map<std::string, std::vector<Polygon_2>> slicesDiffGeometryPolygon) {
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
			//     offset_polygons.clear();
		}
		result.insert(std::pair<std::string, std::vector<Polygon_with_holes_ptr_vector>>(it->first, offsets));
		//   offsets.clear();
	};

	return result;

	/*
	for (std::pair<std::string, std::vector<Polygon_2>> polygons : slicesDiffGeometryPolygon) {
		//Polygon_with_holes_ptr_vector offset_polygons = CGAL::create_interior_skeleton_and_offset_polygons_2(offset, element);
		std::cout << "------------------------------------------------------" << std::endl;
		std::cout << "Computing Slice at z= " + polygons.first << std::endl;
		for (int i = 0; i < polygons.second.size(); ++i) {
			Polygon_with_holes p_hole(polygons.second[i]);
			Polygon_with_holes_ptr_vector offset_polygons = CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(offset, p_hole);
			/*
			// Temporär zur Visualisierung speichern als eps file
			std::string zPosition = "_offsetAt_z_" + polygons.first + "(" + std::to_string(i) + ")";
			std::string eps_name = filename + zPosition + ".eps";
			std::ofstream eps(eps_name.c_str());
			if (eps)
			{
				dump_to_eps(p_hole, offset_polygons, eps);
			}
			else
			{
				std::cerr << "Could not open result file: " << eps_name << std::endl;
			}*/

			// Prüfen jedes Punktes
		   /* for (int iz = 0; iz < offset_polygons.size(); ++iz) {
				Polygon_with_holes poly = *offset_polygons[iz].get();

				for (std::vector<Point>::const_iterator it = poly.outer_boundary().begin(); it != poly.outer_boundary().end(); ++it) {
					Point xycord = *it;
					double zcord = std::stod(polygons.first);
					//millingTool =
					Mesh millingToolTemp = positioningMillingTool(millingTool, xycord, zcord);
					bool intersectingGeometry = CGAL::Polygon_mesh_processing::do_intersect(inputMesh, millingToolTemp);
					if (intersectingGeometry) {
						std::cout << "millingtool intersect with product geometry: " << CGAL::Polygon_mesh_processing::do_intersect(inputMesh, millingToolTemp) << std::endl;
						Nef_polyhedron miNef(millingToolTemp);
						Nef_polyhedron inNef(inputMesh);
						Nef_polyhedron test = inNef - miNef;
						Polyhedron_3 test_p;
						test.boundary().convert_to_Polyhedron(test_p);
						Mesh test_m;
						CGAL::convert_nef_polyhedron_to_polygon_mesh(test, test_m);

						std::string zPosition = "_intersection_z_" + std::to_string(zcord) + "(x_" + std::to_string(xycord.x().exact().to_double()) + "_y_" + std::to_string(xycord.y().exact().to_double()) + ")";
						std::string eps_name = filename + zPosition + ".off";
						//                    std::ofstream out(eps_name.c_str());

						std::ofstream out;
						out.open(eps_name.c_str());
						out << test_m;
						out.close();
					};
				};
			};
		};
	};
	*/
};