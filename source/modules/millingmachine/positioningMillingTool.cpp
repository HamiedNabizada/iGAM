#include "positioningMillingTool.h"
/*
Positioniert Milling Tool
*/
Mesh positioningMillingTool(Mesh toolGeometry, Point xy, double z, bool saveToolPositions, std::filesystem::path out) {
	CGAL::Bbox_3 bbMillingTool = CGAL::Polygon_mesh_processing::bbox(toolGeometry);
	// Ermittlung Mittelpunkt
	double x_current = (bbMillingTool.xmax() + bbMillingTool.xmin()) / 2;
	double y_current = (bbMillingTool.xmax() + bbMillingTool.xmin()) / 2;
	double z_current = bbMillingTool.zmin();
	Vector_3 transvec(xy.x() - x_current, xy.y() - y_current, z - z_current);
	Aff_transformation_3 transl(CGAL::TRANSLATION, transvec);
	CGAL::Polygon_mesh_processing::transform(transl, toolGeometry);
	if (saveToolPositions) {
		std::filesystem::path outputPath = out;
		outputPath /= "toolPositions";
		std::filesystem::create_directories(outputPath);
		std::string filename = "x_" + std::to_string(xy.x().exact().to_double()) + "_y_" + std::to_string(xy.y().exact().to_double()) + "_z_" + std::to_string(z) + ".off";
		std::ofstream outstream(outputPath / filename);
		CGAL::write_off(outstream, toolGeometry);
		outstream.close();
	}
	return toolGeometry;
};

// TODO: Herausfinden wie ich neu positionieren kann
void checkPosition(Mesh inputGeometry, Mesh toolgeometry, Point_3 checkPoint, bool saveIntersections, std::filesystem::path out) {
	Mesh intersection;
	CGAL::Polygon_mesh_processing::corefine_and_compute_intersection(inputGeometry, toolgeometry, intersection);
	if (!intersection.is_empty()) {
		double volume = CGAL::Polygon_mesh_processing::volume(intersection).exact().to_double();
		std::string cpx = std::to_string(checkPoint.exact().x().to_double());
		std::string cpy = std::to_string(checkPoint.exact().y().to_double());
		std::string cpz = std::to_string(checkPoint.exact().z().to_double());

		std::cout << "Intersection with geometry at check point: (x=" << cpx << "), (y=" << cpy << "), (z=" << cpz << "), Volume=" << std::to_string(volume) << std::endl;
		if (saveIntersections) {
			std::filesystem::path outputpath = out;
			outputpath /= "intersections";
			std::filesystem::create_directories(outputpath);
			std::string filename = "i_x" + cpx + "_y" + cpy + "_z" + cpz + "_vol" + std::to_string(volume) + ".off";
			std::ofstream outstream(outputpath / filename);
			CGAL::write_off(outstream, intersection);
			outstream.close();
		}

	};
};

