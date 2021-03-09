#pragma once
#include <iostream>
#include <fstream>
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h> // Kernel
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/transform.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/aff_transformation_tags.h>

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <boost/shared_ptr.hpp>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/convex_hull_3.h>
#include <functional>

#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/minkowski_sum_3.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>

//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel   K;
typedef K::Point_3                                          Point_3;
typedef K::Point_2                                          Point;
typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;
typedef K::Vector_3                                         Vector_3;
typedef CGAL::Aff_transformation_3<K>                       Aff_transformation_3;

typedef CGAL::Polygon_2<K>                                  Polygon_2;
typedef CGAL::Polygon_with_holes_2<K>                       Polygon_with_holes;
typedef boost::shared_ptr<Polygon_with_holes>               Polygon_with_holes_ptr;
typedef std::vector<Polygon_with_holes_ptr>                 Polygon_with_holes_ptr_vector;
typedef K::FT FT;
typedef K::Ray_3 Ray;
typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;
typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
typedef CGAL::AABB_face_graph_triangle_primitive<Mesh> Primitive;
typedef CGAL::AABB_traits<K, Primitive> Traits;
typedef CGAL::AABB_tree<Traits> Tree;
typedef boost::optional<Tree::Intersection_and_primitive_id<Ray>::Type> Ray_intersection;

typedef CGAL::Nef_polyhedron_3<K>     Nef_polyhedron;
Mesh positioningMillingTool(Mesh toolGeometry, Point xy, double z);

void checkPosition(Mesh inputGeometry, Mesh toolgeometry, Point_3 checkPoint);
