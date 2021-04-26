#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_2.h>
#include <boost/shared_ptr.hpp>

#include "config.h"
#include "enums.h"
#include "helpFunctions.h"
#include "rawMaterial.h"
#include "alignGeometry.h"
#include "config.h"
#include "geometrySlicing.h"
#include "computeOffset.h"
#include "removableVolume.h"
#include "millingTool.h"

typedef CGAL::Exact_predicates_exact_constructions_kernel	K;
typedef K::Point_3											Point_3;
typedef CGAL::Surface_mesh<K::Point_3>						Mesh;
typedef CGAL::Polygon_2<K>                                  Polygon_2;
typedef CGAL::Polygon_with_holes_2<K>                       Polygon_with_holes;
typedef boost::shared_ptr<Polygon_with_holes>               Polygon_with_holes_ptr;
typedef std::vector<Polygon_with_holes_ptr>                 Polygon_with_holes_ptr_vector;
typedef std::vector<std::vector<Point_3>>					OffsetContainer;

class ManufacturabilityAnalysis
{
public:
	ManufacturabilityAnalysis(Config& config, std::filesystem::path outputPath, Direction direction, RawMaterial rawMaterial, Mesh workingPieceGeometry, MillingTool millingTool);
	Direction direction;
	std::filesystem::path outputPath;

private:
	std::map<std::string, std::vector<Polygon_2>> slicesDiffGeometryPolygon;
	std::map<std::string, std::vector<OffsetContainer>> offsets;
	std::vector<Mesh> removableVolumes;
	Mesh workingPieceConvexHull;
	Mesh diffGeometry;
};
