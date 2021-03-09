#include "positioningMillingTool.h"
/*
Positioniert Milling Tool
*/
Mesh positioningMillingTool(Mesh toolGeometry, Point xy, double z) {
	CGAL::Bbox_3 bbMillingTool = CGAL::Polygon_mesh_processing::bbox(toolGeometry);
	// Ermittlung Mittelpunkt
	double x_current = (bbMillingTool.xmax() + bbMillingTool.xmin()) / 2;
	double y_current = (bbMillingTool.xmax() + bbMillingTool.xmin()) / 2;
	double z_current = bbMillingTool.zmin();
	Vector_3 transvec(xy.x() - x_current, xy.y() - y_current, z - z_current);
	Aff_transformation_3 transl(CGAL::TRANSLATION, transvec);
	CGAL::Polygon_mesh_processing::transform(transl, toolGeometry);
	return toolGeometry;
};

// TODO: Herausfinden wie ich neu positionieren kann
void checkPosition(Mesh inputGeometry, Mesh toolgeometry, Point_3 checkPoint) {
	Mesh intersection;
	CGAL::Polygon_mesh_processing::corefine_and_compute_intersection(inputGeometry, toolgeometry, intersection);
	if (!intersection.is_empty()) {
		double volume = CGAL::Polygon_mesh_processing::volume(intersection).exact().to_double();
		std::string eps_name = "intersection_x_" + std::to_string(checkPoint.exact().x().to_double()) + "_y_" + std::to_string(checkPoint.exact().y().to_double()) +
			"_z_" + std::to_string(checkPoint.exact().z().to_double()) + "_" + std::to_string(rand()) + ".off";
		std::ofstream out;
		out.open(eps_name.c_str());
		CGAL::write_off(out, intersection);
	};
};

