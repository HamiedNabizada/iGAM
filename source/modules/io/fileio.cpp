#include "fileio.h"
/*
Importiert Off File
*/

Mesh importerOff(const char* filename, bool print_information) {
    Mesh mesh;
    std::ifstream filestream(filename);
    if (!filestream || !(filestream >> mesh) || mesh.is_empty()
        || !CGAL::is_triangle_mesh(mesh)) {
        std::cerr << "Not a valid input file." << std::endl;
    };
    if (print_information) {
        std::cout << "Filename: " << filename << std::endl;
        std::cout << "Number of vertices " << mesh.number_of_vertices() << std::endl;
        std::cout << "Number of edges " << mesh.number_of_edges() << std::endl;
        std::cout << "Number of faces " << mesh.number_of_faces() << std::endl;
        std::cout << "Volume " << CGAL::Polygon_mesh_processing::volume(mesh) << std::endl << std::endl;
    };
    return mesh;
};

Mesh importerOff(std::filesystem::path path, bool print_information) {
    Mesh mesh;
    std::ifstream filestream(path);
    if (!filestream || !(filestream >> mesh) || mesh.is_empty()
        || !CGAL::is_triangle_mesh(mesh)) {
        std::cerr << "Not a valid input file." << std::endl;
    };
    if (print_information) {
        std::cout << "Filename: " << path.filename() << std::endl;
        std::cout << "Number of vertices " << mesh.number_of_vertices() << std::endl;
        std::cout << "Number of edges " << mesh.number_of_edges() << std::endl;
        std::cout << "Number of faces " << mesh.number_of_faces() << std::endl;
        std::cout << "Volume " << CGAL::Polygon_mesh_processing::volume(mesh) << std::endl << std::endl;
    };
    return mesh;
};