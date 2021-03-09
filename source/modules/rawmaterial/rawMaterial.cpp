#include "rawMaterial.h"

RawMaterial::RawMaterial(json jsonData, bool printInformation)
{
	id = jsonData.at("id");
	quantity = jsonData.at("quantity");
	loadDimensions(jsonData.at("dimension"));
	loadMaterialData(jsonData.at("Material"));
	if (printInformation) {
		printRawMaterialInformation();
	};
};

void RawMaterial::loadDimensions(json dimensionData)
{
	height[0] = dimensionData.at("Height").at("MinDimension");
	height[1] = dimensionData.at("Height").at("MaxDimension");
	width[0] = dimensionData.at("Width").at("MinDimension");
	width[1] = dimensionData.at("Width").at("MaxDimension");
	length[0] = dimensionData.at("Length").at("MinDimension");
	length[1] = dimensionData.at("Length").at("MaxDimension");

};

void RawMaterial::loadMaterialData(json materialData)
{
	materialName = materialData.at("name");

};

void RawMaterial::printRawMaterialInformation() {
	std::cout << "Imported raw material " << std::endl;
	std::cout << "ID: " << id << std::endl;
	std::cout << "Height: " << height[0] << ", " << height[1] << std::endl;
	std::cout << "Width: " << width[0] << ", " << width[1] << std::endl;
	std::cout << "Length: " << length[0] << ", " << length[1] << std::endl;
	std::cout << "MaterialName: " << materialName << std::endl;
}
