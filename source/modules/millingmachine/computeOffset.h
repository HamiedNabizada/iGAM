#pragma once
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/create_offset_polygons_2.h>
#include <CGAL/create_offset_polygons_from_polygon_with_holes_2.h>
#include <boost/shared_ptr.hpp>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel   K;

typedef K::Point_3                                          Point_3;
typedef K::Point_2                                          Point;
typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;
typedef CGAL::Polygon_2<K>                                  Polygon_2;
typedef CGAL::Polygon_with_holes_2<K>                       Polygon_with_holes;

typedef boost::shared_ptr<Polygon_with_holes>               Polygon_with_holes_ptr;
typedef std::vector<Polygon_with_holes_ptr>                 Polygon_with_holes_ptr_vector;

/*
Berechnet Offset, gibt Map zur�ck Key=Z-Koordinate, Value=Vector mit allen Offesets an dieser Stelle.
*/
std::map<std::string, std::vector<Polygon_with_holes_ptr_vector>> computeOffset(double offset, std::map<std::string, std::vector<Polygon_2>> slicesDiffGeometryPolygon);