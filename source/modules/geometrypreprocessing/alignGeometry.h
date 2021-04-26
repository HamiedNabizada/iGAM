#pragma once
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h> 
#include <CGAL/Polygon_mesh_processing/transform.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/aff_transformation_tags.h>
#include <CGAL/Lazy_exact_nt.h>
#include <boost/math/constants/constants.hpp>
#include "enums.h"

typedef CGAL::Exact_predicates_exact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;
typedef K::Vector_3                                         Vector_3;
typedef CGAL::Aff_transformation_3<K>						Aff_transformation_3;
typedef CGAL::Lazy_exact_nt<double>							NT;


/** 
* Zentriert Geometrie und richtet nach Z aus:
* Toolgeometrie niedrigster Punkt z=0 (millingTool = true)
* Werkstückgeometrie höchster Punkt z=0 (millingTool = false)
* 
*/
Mesh centerGeometry(Mesh geometry, bool millingTool);
/**
 * Richtet MillingTool an Geometrie des Werkstücks aus gemäß direction
 * 
 */
Mesh alignMillingTool(Mesh millingTool, Mesh WorkingPieceGeometry, Direction direction);

/**
 * Erzeugt Transformationsmatrix für Rotation: https://en.wikipedia.org/wiki/Transformation_matrix#Affine_transformations
 */
Aff_transformation_3 buildTranslationMatrix(Direction direction);
