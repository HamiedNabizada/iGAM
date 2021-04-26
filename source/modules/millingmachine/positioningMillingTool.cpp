#include "positioningMillingTool.h"
/*
Positioniert Milling Tool
*/
//old
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
		std::filesystem::path outputPath = out / "toolPositions";
		std::filesystem::create_directories(outputPath);
		std::string filename = "x_" + std::to_string(xy.x().exact().to_double()) + "_y_" + std::to_string(xy.y().exact().to_double()) + "_z_" + std::to_string(z) + ".off";
		std::ofstream outstream(outputPath / filename);
		CGAL::write_off(outstream, toolGeometry);
		outstream.close();
	}
	return toolGeometry;
};


Mesh positioningMillingTool(std::ofstream& debuger, Mesh toolGeometry, Point_3 point) {
	CGAL::Bbox_3 bbMillingTool = CGAL::Polygon_mesh_processing::bbox(toolGeometry);
	// Ermittlung Mittelpunkt
	double x_current = (bbMillingTool.xmax() + bbMillingTool.xmin()) / 2;
	double y_current = (bbMillingTool.xmax() + bbMillingTool.xmin()) / 2;
	double z_current = bbMillingTool.zmin();
	// Verschieben zu point
	Vector_3 transvec(point.x() - x_current, point.y() - y_current, point.z() - z_current);
	Aff_transformation_3 transl(CGAL::TRANSLATION, transvec);
	CGAL::Polygon_mesh_processing::transform(transl, toolGeometry);
	/*
	if (saveToolPositions) {
		std::filesystem::path outputPath = out / "toolPositions";
		std::filesystem::create_directories(outputPath);
		std::string filename = "x_" + std::to_string(point.x().exact().to_double()) + "_y_" + std::to_string(point.y().exact().to_double()) + "_z_" + std::to_string(point.z().exact().to_double()) + ".off";
		std::ofstream outstream(outputPath / filename);
		CGAL::write_off(outstream, toolGeometry);
		outstream.close();
	}*/
	return toolGeometry;
};
// TODO: Herausfinden wie ich neu positionieren kann
bool checkPosition(std::ofstream& debuger, Mesh inputGeometry, Mesh toolgeometry, double tooldiameter, Point_3& checkPoint, bool saveIntersections, std::filesystem::path out) {
	toolgeometry = positioningMillingTool(debuger,toolgeometry, checkPoint);
	bool valid = true;
	// 2 Versuche um zu prüfen ob sich ein Punkt finden lässt, der keine Intersection verursacht
	if (!computeIntersection(debuger, inputGeometry, toolgeometry, tooldiameter, checkPoint)) {
		toolgeometry = positioningMillingTool(debuger, toolgeometry, checkPoint);
		if (!computeIntersection(debuger, inputGeometry, toolgeometry, tooldiameter, checkPoint)) {
			valid = false;
		};
	};
	if (valid) debuger << "checkpoint valid" << std::endl;
	if (!valid) debuger << "checkpoint invalid" << std::endl;


	return valid;
};

bool computeIntersection(std::ofstream& debuger, Mesh inputGeometry, Mesh toolgeometry, double tooldiameter, Point_3& checkPoint) {
	double radius = tooldiameter / 2.0;
	bool valid = true;
	Mesh intersection;
	CGAL::Polygon_mesh_processing::corefine_and_compute_intersection(inputGeometry,toolgeometry, intersection);
	if (!intersection.is_empty()) {
		double volume = CGAL::Polygon_mesh_processing::volume(intersection).exact().to_double();
		if (volume > 0.00005) { // Kleine Toleranz für Rundungsfehler TODO: besseren Wert finden
			valid = false;
			double cpx = checkPoint.exact().x().to_double();
			double cpy = checkPoint.exact().y().to_double();
			double cpz = checkPoint.exact().z().to_double();
			Tree intersectionTree(faces(intersection).first, // Transformieren zu AABB tree für Distance Computation
				faces(intersection).second,
				intersection);
			Point_3 clp = intersectionTree.closest_point(checkPoint);
			double clx = clp.exact().x().to_double();
			double cly = clp.exact().y().to_double();
			double clz = clp.exact().z().to_double(); //TODO: Später diesen Punkt mit einbeziehen, wenn Spannfutter mit berücksichtigt wird


		
			double distH = sqrt((clx-cpx)*(clx-cpx)+(cly-cpy)*(cly-cpy)); // Distanz zwischen CheckPoint und closest point
			double distMove = radius - distH;							  // Versatz um den Punkt verschoben werden muss
			double newX = cpx < clx ? cpx - distMove : cpx + distMove; 			//TODO: Das ist nur eine Annäherung, besser über Winkel lösen
			double newY = cpy < cly ? cpy - distMove : cpy + distMove;


			debuger << "_______________________________________________________" << std::endl;
			debuger << "radius: " << radius << " distH: " << distH << " distMove: " << distMove << std::endl;
			debuger << "checkPoint   x: " << cpx << " y: " << cpy << " z: " << cpz << std::endl;
			debuger << "closestPoint x: " << clx << " y: " << cly << " z: " << clz << std::endl;
			debuger << "new          x: " << newX << " y: " << newY  << std::endl;
			checkPoint =  Point_3(newX, newY, cpz);
			//std::cout << "Intersection with geometry at check point: (x=" << cpx << "), (y=" << cpy << "), (z=" << cpz << "), Volume=" << std::to_string(volume) << std::endl;
			/*if (saveIntersections) {
				std::filesystem::path outputpath = out;
				outputpath /= "intersections";
				std::filesystem::create_directories(outputpath);
				std::string filename = "i_x" + std::to_string(cpx) + "_y" + std::to_string(cpy) + "_z" + std::to_string(cpz) + "_vol" + std::to_string(volume) + ".off";
				std::ofstream outstream(outputpath / filename);
				CGAL::write_off(outstream, intersection);
				outstream.close();
			}*/
		}
	}
	return valid;


}
