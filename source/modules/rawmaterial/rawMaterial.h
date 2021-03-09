#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
typedef CGAL::Exact_predicates_exact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;

class RawMaterial
{
public:
	RawMaterial(json jsonData, bool printInformation);
	int quantity;
	std::string id;
	double height[2];
	double width[2];
	double length[2];
	std::string materialName;

	Mesh geometry;
	double volume;
private:
	void loadDimensions(json dimensionData);
	void loadMaterialData(json materialData);
	void printRawMaterialInformation();
};
