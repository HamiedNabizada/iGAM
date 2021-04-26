#pragma once
#include <vector>


//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h> // Kernel
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h> 
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_mesh_slicer.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Bbox_3.h>
#include "enums.h"


//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;
typedef K::Point_3                                          Point_3;
typedef K::Point_2                                          Point;
typedef CGAL::Polygon_2<K>                                  Polygon_2;
typedef std::vector<K::Point_3>                             Polyline_type;
typedef std::list< Polyline_type >                          Polylines;

/*
1. Berechnet CuttingDistance (Kleinster Face des Meshs dividiert durch 2)
2. Zerlegt Geometrie in Slices im Abstand zuvor berechneter CuttingDistance
*/
std::map<std::string, std::vector<Polygon_2>> sliceDiffGeometry(Mesh difGeometry, Direction direction);
