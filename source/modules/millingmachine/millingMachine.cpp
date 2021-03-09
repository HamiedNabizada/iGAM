#include "millingMachine.h"

MillingMachine::MillingMachine(std::filesystem::path pathJsonFile, bool printInformation) {
	std::cout << "Loading milling machine from file: " << pathJsonFile.string() << std::endl;
	std::ifstream filestream(pathJsonFile.c_str());
	json jsonData;
	filestream >> jsonData;
	filestream.close();
	serializeGeneralInformation(jsonData.at("generalInformation"));
	serializeTechnicalInformation(jsonData.at("technicalData"));
	std::string toolFile = jsonData.at("tools").at("filename");
	std::filesystem::path pathToolFile = pathJsonFile.replace_filename(toolFile);
	std::cout << "path Tool File: " << pathToolFile << std::endl;
	loadToolGeometry(pathToolFile);
	if (printInformation) {
		printGeneralInformation();
		printTechnicalInformation();
	};
};

void MillingMachine::checkManufacturability(Mesh workingPieceGeometry, RawMaterial rawMaterial) {
	std::filesystem::path outputPath = std::filesystem::current_path();
	
	outputPath /= this->company;
	outputPath /= this->series;
	std::filesystem::create_directories(outputPath);

	// Step0: Check if rawMaterial fit
	rawMaterial.geometry = centerGeometry(rawMaterial.geometry, false);
	
	std::ofstream ofs(outputPath / "selectedRawMaterial.off");
	CGAL::write_off(ofs, rawMaterial.geometry);
	ofs.close();

	// Step1: Compute Convex hull of workingPieceGeometry
	Mesh convexHullMesh;
	CGAL::convex_hull_3(workingPieceGeometry.points().begin(), workingPieceGeometry.points().end(), convexHullMesh);
	ofs.open(outputPath / "convexHull.off");
	CGAL::write_off(ofs, convexHullMesh);
	ofs.close();
	// Step2: Compute Difference Geometry
	Mesh diffGeometryMesh;
	CGAL::Polygon_mesh_processing::corefine_and_compute_difference(convexHullMesh, workingPieceGeometry, diffGeometryMesh);
	ofs.open(outputPath / "diffGeometry.off");
	CGAL::write_off(ofs, diffGeometryMesh);
	ofs.close();
	// Step3: Slice DiffGeometry
	std::map<std::string, std::vector<Polygon_2>> slicesDiffGeometryPolygon = sliceDiffGeometry(diffGeometryMesh);

	// Step4: Compute Offset
	std::map<std::string, std::vector<Polygon_with_holes_ptr_vector>> offsets = computeOffset(millingTool.diameter / 2.0, slicesDiffGeometryPolygon);

	// Step5: Minkowski Summen der abtragbaren volumen erstellen (TODO: Richtige CD implementieren)
	std::vector<Mesh> removableVolumes = computeRemovableVolumes(offsets, workingPieceGeometry, millingTool.toolGeometry, convexHullMesh);

	// Step6: Compute final geometry
	computeLeftover(removableVolumes, convexHullMesh);

};

void MillingMachine::loadToolGeometry(std::filesystem::path pathToolFile) {
	millingTool.loadMillingTool(pathToolFile);

	std::cout << millingTool.diameter << std::endl;
};

void MillingMachine::serializeGeneralInformation(json object) {
	company = object.value("company", "No company name specified");
	series = object.value("series", "No series specified");
	machineType = object.value("machinetype", "No machine type specified");
	applicationArea = object.value("applicationarea", "No application area specified");
};

void MillingMachine::serializeTechnicalInformation(json object) {
	json tAxis = object.at("axis");
	std::cout << tAxis.dump(4) << std::endl;
	travelPathX = std::tuple<double, std::string>(tAxis.at("x").value("travelPath", 0), tAxis.at("x").value("dimension", "undefined"));
	travelPathY = std::tuple<double, std::string>(tAxis.at("y").value("travelPath", 0), tAxis.at("y").value("dimension", "undefined"));
	travelPathZ = std::tuple<double, std::string>(tAxis.at("z").value("travelPath", 0), tAxis.at("z").value("dimension", "undefined"));
};

void MillingMachine::printGeneralInformation() {
	std::cout << "company: " << company << std::endl;
	std::cout << "series: " << series << std::endl;
	std::cout << "machine type: " << machineType << std::endl;
	std::cout << "application area: " << applicationArea << std::endl;
};

void MillingMachine::printTechnicalInformation() {
	std::cout << "Technical Information (Axis): " << std::endl;
	std::cout << "X: " << std::get<0>(travelPathX) << " " << std::get<1>(travelPathX) << std::endl;
	std::cout << "Y: " << std::get<0>(travelPathY) << " " << std::get<1>(travelPathY) << std::endl;
	std::cout << "Z: " << std::get<0>(travelPathZ) << " " << std::get<1>(travelPathZ) << std::endl;
};