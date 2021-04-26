#include "computeOffset.h"

std::map<std::string, std::vector<OffsetContainer>> computeOffset(Direction direction, double offset, Mesh convexHull,
																				std::map<std::string, std::vector<Polygon_2>> slicesDiffGeometryPolygon, 
																				bool saveEPSFiles, std::filesystem::path out) {

	std::filesystem::path outputPath = out;
	if (saveEPSFiles) { outputPath /= "epsfiles"; std::filesystem::create_directories(outputPath); };
	CGAL::Bbox_3 convexHullBBox = CGAL::Polygon_mesh_processing::bbox(convexHull);
	 
 //	std::map<std::string, std::vector<Polygon_with_holes_ptr_vector>> result;
	std::map<std::string, std::vector<OffsetContainer>> result;
	// Rückwärts iterieren
	for (std::map<std::string, std::vector<Polygon_2>>::reverse_iterator it = slicesDiffGeometryPolygon.rbegin(); it != slicesDiffGeometryPolygon.rend(); ++it) {
		std::cout << "Computing Offset for Slice at z= " + it->first << std::endl;
		double z = std::stod(it->first);
		std::vector<Polygon_2> polyvec = it->second;
	//	std::vector<Polygon_with_holes_ptr_vector> offsets;
		std::vector<OffsetContainer> offsets;
		for (int i = 0; i < polyvec.size(); ++i) {
			Polygon_with_holes p_hole(polyvec[i]);
			Polygon_with_holes_ptr_vector offset_polygons = CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(offset, p_hole);
			
			if (offset_polygons.size() > 0) {
				OffsetContainer res = repositionOffsets(direction, offset_polygons, convexHullBBox, p_hole, offset, z);
				offsets.push_back(res);
			//	offsets.push_back(offset_polygons);
				/* EPS Files gibt es erstmal nicht
				if (saveEPSFiles) {
					std::string filename = "_offsetAt_" + it->first + "(" + std::to_string(i) + ").eps";
					std::ofstream eps(outputPath / filename);
					std::cout << "filename: " << filename << std::endl;
					if (eps) {dump_to_eps(p_hole, offset_polygons, eps);}
					else { std::cerr << "Could not open result file: " << filename << std::endl; }
					eps.close();
				}*/
			};
		}
		result.insert(std::pair<std::string, std::vector<OffsetContainer>>(it->first, offsets));
	//	result.insert(std::pair<std::string, std::vector<Polygon_with_holes_ptr_vector>>(it->first, offsets));
	};

	return result;
};
// Prüft ob Offsetpunkte verschoben werden müssen, gibt Ergebnisse der Offsetberechnung als Punktemenge heraus.
OffsetContainer repositionOffsets(Direction direction, Polygon_with_holes_ptr_vector offset_polygons, 
								  CGAL::Bbox_3 convexHullBBox, 
								  Polygon_with_holes p_hole, 
								  double offset, double zpos) {

	OffsetContainer result;
	if(direction == Direction::z_negative || direction == Direction::z_positive){
		double xMin, xMax, yMin, yMax;
		bool checkXmin = false, checkXmax = false, checkYmin = false, checkYmax = false;
		CGAL::Bbox_2 p_holeBBox = p_hole.bbox();
		if (almost_equal(p_holeBBox.xmin(), convexHullBBox.xmin())) { xMin = convexHullBBox.xmin(); checkXmin = true; };
		if (almost_equal(p_holeBBox.xmax(), convexHullBBox.xmax())) { xMax = convexHullBBox.xmax(); checkXmax = true; };
		if (almost_equal(p_holeBBox.ymin(), convexHullBBox.ymin())) { yMin = convexHullBBox.ymin(); checkYmin = true; };
		if (almost_equal(p_holeBBox.ymax(), convexHullBBox.ymax())) { yMax = convexHullBBox.ymax(); checkYmax = true; };
		for (int i = 0; i < offset_polygons.size(); ++i) {
			Polygon_with_holes *poly = offset_polygons[i].get(); 
			std::vector<Point_3> points;
			bool insertedBefore = false;
			for (std::vector<Point>::const_iterator it = poly->outer_boundary().begin(); it != poly->outer_boundary().end(); ++it) {
				Point xy = *it;
				double x_tmp = xy.exact().x().to_double();
				double y_tmp = xy.exact().y().to_double();
				bool insertNew = false;
				if (checkXmax && almost_equal(xMax, x_tmp + offset)) { x_tmp = xMax - (offset / 100.0); insertNew = true; };
				if (checkXmin && almost_equal(xMin, x_tmp - offset)) { x_tmp = xMin + (offset / 100.0); insertNew = true; };
				if (checkYmin && almost_equal(yMin, y_tmp - offset)) { y_tmp = yMin + (offset / 100.0); insertNew = true; };
				if (checkYmax && almost_equal(yMax, y_tmp + offset)) { y_tmp = yMax - (offset / 100.0); insertNew = true; };
				// Fängt Problematik für schrägen Verlauf ab (Vorheriger Punkt wird mit aufgenommen)
				/*
				if (!insertNew) {
					points.push_back(Point_3(x_tmp, y_tmp, zpos));
				}
				else if (!insertedBefore) {
					points.push_back(Point_3(xy.exact().x().to_double(), xy.exact().y().to_double(), zpos));
					points.push_back(Point_3(x_tmp, y_tmp, zpos));
					insertedBefore = true;
				}
				else {
					points.push_back(Point_3(x_tmp, y_tmp, zpos));
					points.push_back(Point_3(xy.exact().x().to_double(), xy.exact().y().to_double(), zpos));
					insertedBefore = false;
				}*/
				points.push_back(Point_3(x_tmp, y_tmp, zpos));

			};
			result.push_back(points);
			//offset_polygons[i].reset();
			//poly->clear();
			/* Code um aus Polylines Mesh zu erzeugen
			typedef CGAL::Triple<int, int, int> Triangle_int;
			std::vector<Triangle_int> patch;
			patch.reserve(points.size() - 2); // there will be exactly n-2 triangles in the patch
			CGAL::Polygon_mesh_processing::triangulate_hole_polyline(points, std::back_inserter(patch));
			Mesh output;
			for (std::size_t i = 0; i < patch.size(); ++i)
			{
				Mesh::Vertex_index u = output.add_vertex(points[patch[i].first]);
				Mesh::Vertex_index v = output.add_vertex(points[patch[i].second]);
				Mesh::Vertex_index w = output.add_vertex(points[patch[i].third]);
				output.add_face(u, v, w);
			};
			std::string filename = "rpplane_" + std::to_string(i) + "_(z_" + std::to_string(zpos) + "_" + std::to_string(std::rand()) + ").off";
			std::ofstream outstream(filename);
			CGAL::write_off(outstream, output);
			outstream.close(); 
			 */

			
		};
		return result;
	}
	else {
		return result;
	}
};

bool almost_equal(double v1, double v2) {
	return std::fabs(v1 - v2) <= std::numeric_limits<double>::epsilon() * 2000000;
		//|| std::fabs(v1 - v2) < std::numeric_limits<double>::min();
};