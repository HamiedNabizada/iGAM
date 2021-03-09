#pragma once
#include <iostream>
#include <fstream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

#include <CGAL/Polygon_with_holes_2.h>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>

#include <CGAL/convex_hull_3.h>

#include "positioningMillingTool.h"

//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
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

std::vector<Mesh> computeRemovableVolumes(std::map<std::string, std::vector<Polygon_with_holes_ptr_vector>> offsets, Mesh workingPieceGeometry, Mesh toolGeometry, Mesh convexHull);
Mesh computeMinkowskiSum(Mesh plane, Mesh toolGeometry);
Mesh computeLeftover(std::vector<Mesh> minkowskiSums, Mesh convexHull);
