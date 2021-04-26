#pragma once
#include <filesystem>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/create_offset_polygons_from_polygon_with_holes_2.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "enums.h"
#include "dump_to_eps.h"

typedef CGAL::Exact_predicates_exact_constructions_kernel   K;

typedef K::Point_3                                          Point_3;
typedef K::Point_2                                          Point;
typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;
typedef CGAL::Polygon_2<K>                                  Polygon_2;
typedef CGAL::Polygon_with_holes_2<K>                       Polygon_with_holes;

typedef boost::shared_ptr<Polygon_with_holes>               Polygon_with_holes_ptr;
typedef std::vector<Polygon_with_holes_ptr>                 Polygon_with_holes_ptr_vector;


typedef std::vector<std::vector<Point_3>>					OffsetContainer;




/*
Berechnet Offset, gibt Map zurück Key=Z-Koordinate, Value=Vector mit allen Offesets an dieser Stelle.
*/
std::map<std::string, std::vector<OffsetContainer>> computeOffset(Direction direction, double offset, Mesh convexHull, std::map<std::string, std::vector<Polygon_2>> slicesDiffGeometryPolygon, bool saveEPSFiles, std::filesystem::path out);
//Polygon_with_holes_ptr_vector repositionOffsets(Direction direction, Polygon_with_holes_ptr_vector offset_polygons, CGAL::Bbox_3 convexHullBBox, Polygon_with_holes p_hole, double offset);

OffsetContainer repositionOffsets(Direction direction, Polygon_with_holes_ptr_vector offset_polygons, CGAL::Bbox_3 convexHullBBox, Polygon_with_holes p_hole, double offset, double zpos);

bool almost_equal(double v1, double v2);
