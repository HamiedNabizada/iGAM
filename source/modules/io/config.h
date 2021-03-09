#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Config {
private:
	std::filesystem::path bin_dir = std::filesystem::current_path();
	bool saveRemovableVolumes = false;
	bool saveConvexHull = false;
	bool saveIntersections = false;
	bool savePlanes = false;
	bool saveOffset = false;
	bool saveDiffGeometry = false;
	bool saveSelectedRawMaterial = false;
	bool saveToolPositions = false;
	bool printMachineInformation = false;
	bool printMillingToolInformation = false;
	bool printImportInformation = false;
	bool printRawMaterialInformation = false;
public:
	Config();
	bool getSaveRemovableVolumes() { return saveRemovableVolumes; };
	bool getSaveConvexHull() { return saveConvexHull; };
	bool getSaveInterstions() { return saveIntersections; };
	bool getSavePlanes() { return savePlanes; };
	bool getSaveOffests() { return saveOffset; };
	bool getSaveDiffGeometry() { return saveDiffGeometry; };
	bool getSaveSelectedRawMaterial() { return saveSelectedRawMaterial; };
	bool getSaveToolPositions() { return saveToolPositions; };
	bool getPrintMillingToolInformation() { return printMillingToolInformation; };
	bool getPrintMachineInformation() {return printMachineInformation;};
	bool getPrintImportInformation() { return printImportInformation; };
	bool getPrintRawMaterialInformation() { return printRawMaterialInformation; };
	std::filesystem::path getBinDir() { return bin_dir; };
};
