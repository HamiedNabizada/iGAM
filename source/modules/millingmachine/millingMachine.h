#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

#include <CGAL/convex_hull_3.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

#include <boost/shared_ptr.hpp>

#include "millingTool.h"
#include "geometrySlicing.h"
#include "computeOffset.h"
#include "removableVolume.h"
#include "rawMaterial.h"
#include "alignGeometry.h"
#include "config.h"

using json = nlohmann::json;
typedef CGAL::Exact_predicates_exact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;

typedef CGAL::Polygon_2<K>                                  Polygon_2;

typedef CGAL::Polygon_with_holes_2<K>                       Polygon_with_holes;
typedef boost::shared_ptr<Polygon_with_holes>               Polygon_with_holes_ptr;
typedef std::vector<Polygon_with_holes_ptr>                 Polygon_with_holes_ptr_vector;


class MillingMachine
{
public:
	/**
	*Konstruktor:
	* - jsonFile: Konfigurationsdatei für MillingMachine
	* - printInformation: Ausgabe der eingelesenen Daten
	*/
	MillingMachine(std::filesystem::path pathJsonFile, Config &con);
	MillingTool millingTool;
	void checkManufacturability(Mesh workingPieceGeometry, RawMaterial rawMaterial, Config &config);

private:
	void serializeGeneralInformation(json object);
	void serializeTechnicalInformation(json object);
	void printGeneralInformation();
	void printTechnicalInformation();
	void loadToolGeometry(std::filesystem::path pathToolFile, bool printMillingToolInformation);

	std::string company;
	std::string series;
	std::string machineType;
	std::string applicationArea;
	std::tuple<double, std::string> travelPathX;
	std::tuple<double, std::string> travelPathY;
	std::tuple<double, std::string> travelPathZ;
};