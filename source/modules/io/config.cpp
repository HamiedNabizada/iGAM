#include "config.h"


Config::Config() {
	std::ifstream configstream("config.json");
	json jsonData;
	configstream >> jsonData;
	configstream.close();
	this->saveRemovableVolumes = jsonData.at("saveRemovableVolumes");
	this->saveConvexHull = jsonData.at("saveConvexHull");
	this->saveIntersections = jsonData.at("saveIntersections");
	this->savePlanes = jsonData.at("savePlanes");
	this->saveOffset = jsonData.at("saveOffset");
	this->saveDiffGeometry = jsonData.at("saveDiffGeometry");
	this->saveToolPositions = jsonData.at("saveToolPositions");
	this->saveSelectedRawMaterial = jsonData.at("saveSelectedRawMaterial");
	this->printImportInformation = jsonData.at("printImportInformation");
	this->printMachineInformation = jsonData.at("printMachineInformation");
	this->printMillingToolInformation = jsonData.at("printMillingToolInformation");
	this->printRawMaterialInformation = jsonData.at("printRawMaterialInformation");
};

