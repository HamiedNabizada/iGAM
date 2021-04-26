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
	return geometry;
}

Mesh alignMillingTool(Mesh millingTool, Mesh WorkingPieceGeometry, Direction direction){
	CGAL::Bbox_3 bboxWPG = CGAL::Polygon_mesh_processing::bbox(WorkingPieceGeometry);
	CGAL::Polygon_mesh_processing::transform(buildTranslationMatrix(direction), millingTool);
	CGAL::Bbox_3 bbox = CGAL::Polygon_mesh_processing::bbox(millingTool);
	double vec_x, vec_y, vec_z;
	if (direction == Direction::z_negative || direction == Direction::z_positive) {
		vec_x = ((bbox.xmax() - bbox.xmin()) / 2.0) - bbox.xmax();
		vec_y = ((bbox.ymax() - bbox.ymin()) / 2.0) - bbox.ymax();
		vec_z = direction == Direction::z_negative ? bboxWPG.zmax() - bbox.zmin() : bboxWPG.zmin() - bbox.zmax();
	};
	if (direction == Direction::y_negative || direction == Direction::y_positive) {
		vec_x = ((bbox.xmax() - bbox.xmin()) / 2.0) - bbox.xmax();
		vec_y = direction == Direction::y_negative ? bboxWPG.ymax() - bbox.ymin() : bboxWPG.ymin() - bbox.ymax();
		vec_z = ((bbox.zmax() - bbox.zmin()) / 2.0) - bbox.zmax();

	};
	if (direction == Direction::x_negative || direction == Direction::x_positive) {
		vec_x = direction == Direction::x_negative ? bboxWPG.xmax() - bbox.xmin() : bboxWPG.xmin() - bbox.xmax();
		vec_y = ((bbox.ymax() - bbox.ymin()) / 2.0) - bbox.ymax();
		vec_z = ((bbox.zmax() - bbox.zmin()) / 2.0) - bbox.zmax();
	};
	Vector_3 transvec(vec_x, vec_y, vec_z);
	Aff_transformation_3 transl(CGAL::TRANSLATION, transvec);
	CGAL::Polygon_mesh_processing::transform(transl, millingTool);
	return millingTool;
}

Aff_transformation_3 buildTranslationMatrix(Direction direction) {
	double theta, cosTheta, sinTheta;
	if (direction == Direction::z_negative || direction == Direction::z_positive || direction == Direction::y_negative || direction == Direction::y_positive) {
		// Drehungen um X-Achse
		theta = direction == Direction::z_negative ? 180 : direction == Direction::y_positive ? 90 : direction == Direction::y_negative ? -90 : 0;
		cosTheta = cos((theta * boost::math::constants::pi<double>()) / 180);
		sinTheta = sin((theta * boost::math::constants::pi<double>()) / 180);
		return Aff_transformation_3(
			NT(1.0), NT(0.0), NT(0.0),
			NT(0.0), NT(cosTheta), NT(-sinTheta),
			NT(0.0), NT(sinTheta), NT(cosTheta),
			NT(1.0)
		);
	};
	if (direction == Direction::x_negative || direction == Direction::x_positive) {
		theta = direction == Direction::x_negative ? -90 :  90;
		cosTheta = cos((theta * boost::math::constants::pi<double>()) / 180);
		sinTheta = sin((theta * boost::math::constants::pi<double>()) / 180);
		return Aff_transformation_3(
			NT(cosTheta), NT(0.0),NT(sinTheta),
			NT(0.0),	  NT(1.0),NT(0.0),
			NT(-sinTheta),NT(0.0),NT(cosTheta),
			NT(1.0)
		);
	}

}
