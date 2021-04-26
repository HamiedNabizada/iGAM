#include "geometrySlicing.h"

std::map<std::string, std::vector<Polygon_2>> sliceDiffGeometry(Mesh diffGeometry, Direction direction) {
	CGAL::Polygon_mesh_slicer<Mesh, K> slicer(diffGeometry);
	CGAL::Bbox_3 bbDiffGeometry = CGAL::Polygon_mesh_processing::bbox(diffGeometry);
	std::map<std::string, std::vector<Polygon_2>> slicesDiffGeometryPolygon;
	Polylines slicesDiffGeometry;
	Polygon_2 p; // Zwischenspeicher
	double cuttingDistance = 10000;// Ermittlung der CuttingDistance. Kleinste Face des Meshes durch 2.
	double maxDiffGeometry;
	double minDiffGeometry;
	int x = 0;
	int y = 0;
	int z = 0;
	if (direction == Direction::z_negative || direction == Direction::z_positive) {
		z = 1;
		std::cout << "if Z_negatvive/positive" << std::endl;
		maxDiffGeometry = bbDiffGeometry.zmax();
		minDiffGeometry = bbDiffGeometry.zmin();
		for (Mesh::Face_index fi : diffGeometry.faces()) {
			CGAL::Bbox_3 fi_test = CGAL::Polygon_mesh_processing::face_bbox(fi, diffGeometry);
			double abs = std::abs(fi_test.zmax() - fi_test.zmin());
			if (abs < cuttingDistance && abs >0) {
				cuttingDistance = abs;
			};
		};
	};
	if (direction == Direction::y_negative || direction == Direction::y_positive) {
		y = 1;
		std::cout << "if Y_negatvive/positive" << std::endl;
		maxDiffGeometry = bbDiffGeometry.ymax();
		minDiffGeometry = bbDiffGeometry.ymin();
		for (Mesh::Face_index fi : diffGeometry.faces()) {
			CGAL::Bbox_3 fi_test = CGAL::Polygon_mesh_processing::face_bbox(fi, diffGeometry);
			double abs = std::abs(fi_test.ymax() - fi_test.ymin());
			if (abs < cuttingDistance && abs >0) {
				cuttingDistance = abs;
			};
		};
	};
	if (direction == Direction::x_negative || direction == Direction::x_positive) {
		x = 1;
		std::cout << "if X_negatvive/positive" << std::endl;
		maxDiffGeometry = bbDiffGeometry.xmax();
		minDiffGeometry = bbDiffGeometry.xmin();
		for (Mesh::Face_index fi : diffGeometry.faces()) {
			CGAL::Bbox_3 fi_test = CGAL::Polygon_mesh_processing::face_bbox(fi, diffGeometry);
			double abs = std::abs(fi_test.xmax() - fi_test.xmin());
			if (abs < cuttingDistance && abs >0) {
				cuttingDistance = abs;
			};
		};
	};

	cuttingDistance = cuttingDistance / 2;
	double cuttingLocation = maxDiffGeometry;
	std::cout << "CUTTIGN DISTANCE: " << cuttingDistance << std::endl;
	std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
	std::cout << "max: " << maxDiffGeometry << " min: " << minDiffGeometry << std::endl;

	while (cuttingLocation >= minDiffGeometry) {
		// Umgekehrtes Vorzeichen, da Ebenengleichung in Koordinatenform
		slicer(K::Plane_3(x, y, z, -cuttingLocation), std::back_inserter(slicesDiffGeometry));
		std::cout << "At CuttingLocation = " << cuttingLocation << ", the slicer intersects " << slicesDiffGeometry.size() << " polylines" << std::endl;
		//Polylines in Polygon_2 überführen
		std::vector<Polygon_2> polygonList; // Zwischenspeicher für PolygonContainer
		for (const auto& result : slicesDiffGeometry) {
			for (const auto& res : result) { p.push_back(Point(res.x(), res.y())); }
			polygonList.push_back(p);
			p.clear();
		};
		slicesDiffGeometryPolygon.insert(std::pair<std::string, std::vector<Polygon_2>>(std::to_string(cuttingLocation), polygonList));
		if (cuttingLocation == minDiffGeometry) { break;};
		cuttingLocation = cuttingLocation - cuttingDistance;
		if (cuttingLocation < minDiffGeometry) {/* Bodengeometrie*/ cuttingLocation = minDiffGeometry;};
		slicesDiffGeometry.clear();
	}
	



	return slicesDiffGeometryPolygon;
};