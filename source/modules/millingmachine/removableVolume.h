#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/minkowski_sum_3.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
// test
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Lazy_exact_nt.h>
#include <CGAL/Nef_nary_union_3.h>
#include <CGAL/Timer.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include "positioningMillingTool.h"
#include "millingTool.h"
#include "config.h"

typedef CGAL::Exact_predicates_exact_constructions_kernel					K;
typedef K::Point_3															Point_3;
typedef K::Point_2															Point;
typedef CGAL::Surface_mesh<K::Point_3>										Mesh;

typedef CGAL::Polygon_with_holes_2<K>										Polygon_with_holes;
typedef boost::shared_ptr<Polygon_with_holes>								Polygon_with_holes_ptr;
typedef std::vector<Polygon_with_holes_ptr>									Polygon_with_holes_ptr_vector;

typedef CGAL::AABB_face_graph_triangle_primitive<Mesh>						Primitive;
typedef CGAL::AABB_traits<K, Primitive>										Traits;
typedef CGAL::AABB_tree<Traits>												Tree;

typedef K::Ray_3															Ray;
typedef boost::optional<Tree::Intersection_and_primitive_id<Ray>::Type>		Ray_intersection;
typedef CGAL::Nef_polyhedron_3<K>											Nef_polyhedron;

typedef std::vector<std::vector<Point_3>>									OffsetContainer;
typedef CGAL::Lazy_exact_nt<double>							NT;
/*
typedef K::FT                                                   FT;
typedef std::array<FT, 3>                                       Custom_point;
typedef std::vector<std::size_t>                                CGAL_Polygon;
*/
std::vector<Mesh> computeRemovableVolumes(std::map<std::string, std::vector<Polygon_with_holes_ptr_vector>> offsets, Mesh workingPieceGeometry, MillingTool millingTool, Mesh convexHull, Config& config, std::filesystem::path out);
std::vector<Mesh> computeRemovableVolumes(std::map<std::string, std::vector<OffsetContainer>> offsets, Mesh workingPieceGeometry, MillingTool millingTool, Mesh convexHull, Config& config, std::filesystem::path out);
Mesh computeMinkowskiSum(Mesh plane, Mesh toolGeometry, Config& config, std::filesystem::path out);
std::pair<bool, Mesh> computeMinkowskiSum(std::ofstream& debuger, std::vector<Point_3> points, MillingTool millingTool, Config& config, std::filesystem::path out);
Mesh computeLeftover(std::vector<Mesh> minkowskiSums, Mesh convexHull, Config& config, std::filesystem::path out);
void setNewZPosition(std::vector<Point_3>& points, double zpos);
