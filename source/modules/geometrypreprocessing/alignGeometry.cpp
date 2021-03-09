#include "alignGeometry.h"

Mesh centerGeometry(Mesh geometry, bool millingTool) {
	CGAL::Bbox_3 bbox = CGAL::Polygon_mesh_processing::bbox(geometry);
	// Ausrichten nach x
	double sidelength_x = (bbox.xmax() - bbox.xmin()) / 2.0; // dieser Wert jeweils in negativer und positiver x-richtung
	double vec_x = sidelength_x - bbox.xmax(); 

	double sidelength_y = (bbox.ymax() - bbox.ymin()) / 2.0;
	double vec_y = sidelength_y - bbox.ymax();

	double vec_z;
	if (millingTool) {
		vec_z = -bbox.zmin();
	}
	else {
		vec_z = -bbox.zmax();
	};

	Vector_3 transvec(vec_x, vec_y, vec_z);
	Aff_transformation_3 transl(CGAL::TRANSLATION, transvec);
	CGAL::Polygon_mesh_processing::transform(transl, geometry);

	return geometry;
}