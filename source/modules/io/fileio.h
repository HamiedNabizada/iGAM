#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h> // Kernel
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel   K;
typedef K::Point_3                                          Point_3;
typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;
/*
Importer für Off-Files
*/
Mesh importerOff(const char* filename, bool print_information);

Mesh importerOff(std::filesystem::path path, bool print_information);