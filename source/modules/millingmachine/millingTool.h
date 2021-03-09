#pragma once
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>

#include "alignGeometry.h"
#include "fileio.h"

using json = nlohmann::json;
typedef CGAL::Exact_predicates_exact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;

class MillingTool
{
public:
	void loadMillingTool(std::filesystem::path pathToolFile);
	Mesh toolGeometry;
	double diameter;

private:
	std::string name;
	CGAL::Bbox_3 bbMillingTool;
};