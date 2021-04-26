#include "manufacturabilityAnalysis.h"
ManufacturabilityAnalysis::ManufacturabilityAnalysis(Config& config, std::filesystem::path outputPath, Direction direction, RawMaterial rawMaterial, Mesh workingPieceGeometry, MillingTool millingTool) {
	// Erstellen der Outputdirectories
	this->direction = direction;
	this->outputPath = outputPath;
	outputPath /= direction_to_string(direction);
	std::filesystem::create_directories(outputPath);

	if (direction == Direction::z_negative || direction == Direction::z_positive) {
	// Step0: Check if rawMaterial fit (TODO)
	rawMaterial.geometry = centerGeometry(rawMaterial.geometry, false);
	std::ofstream ofs;
	if (config.getSaveSelectedRawMaterial()) {
		std::string filename = "selectedRawMaterial_id_" + rawMaterial.id + ".off";
		ofs.open(outputPath / filename);
		CGAL::write_off(ofs, rawMaterial.geometry);
		ofs.close();
	};

	// Step1: Compute Convex hull of workingPieceGeometry
	CGAL::convex_hull_3(workingPieceGeometry.points().begin(), workingPieceGeometry.points().end(), this->workingPieceConvexHull);

	if (config.getSaveConvexHull()) {
		ofs.open(outputPath / "convexHull.off");
		CGAL::write_off(ofs, this->workingPieceConvexHull);
		ofs.close();
	};
	// Step2: Compute Difference Geometry
//	Mesh diffGeometryMesh;
	CGAL::Polygon_mesh_processing::corefine_and_compute_difference(this->workingPieceConvexHull, workingPieceGeometry, this->diffGeometry);
	if (config.getSaveDiffGeometry()) {
		ofs.open(outputPath / "diffGeometry.off");
		CGAL::write_off(ofs, this->diffGeometry);
		ofs.close();
	};

	// Step3: Slice DiffGeometry
	this->slicesDiffGeometryPolygon = sliceDiffGeometry(this->diffGeometry, direction);

	// Step4: Compute Offset
	this->offsets = computeOffset(direction, millingTool.diameter / 2.0, this->workingPieceConvexHull, slicesDiffGeometryPolygon, config.getSaveOffests(), outputPath);

	// Step5: Minkowski Summen der abtragbaren volumen erstellen (TODO: Richtige CD implementieren)
		this->removableVolumes = computeRemovableVolumes(this->offsets, workingPieceGeometry, millingTool, this->workingPieceConvexHull, config, outputPath);

	// Step6: Compute final geometry
	Mesh endgeomerty = computeLeftover(this->removableVolumes, this->workingPieceConvexHull, config, outputPath);
	double volEnd = CGAL::Polygon_mesh_processing::volume(endgeomerty).exact().to_double();
	double volWPG = CGAL::Polygon_mesh_processing::volume(workingPieceGeometry).exact().to_double();
	std::cout << "-> Volume of Endgeomerty: " << volEnd << std::endl;
	std::cout << "-> Volume of working piece: " << volWPG << std::endl;
	std::cout << "-> Non-machinable volume: " << volEnd - volWPG << std::endl;
	std::cout << "-> Working piece is " << (volWPG / volEnd)*100 << "% manufacturable" << std::endl;
	}
};