#include "geometrySlicing.h"

std::map<std::string, std::vector<Polygon_2>> sliceDiffGeometry(Mesh diffGeometry) {
    CGAL::Polygon_mesh_slicer<Mesh, K> slicer(diffGeometry);
    CGAL::Bbox_3 bbDiffGeometry = CGAL::Polygon_mesh_processing::bbox(diffGeometry);
    double zMaxDiffGeometry = bbDiffGeometry.zmax();
    double zMinDiffGeometry = bbDiffGeometry.zmin();
    std::map<std::string, std::vector<Polygon_2>> slicesDiffGeometryPolygon;
    Polylines slicesDiffGeometry;
    Polygon_2 p; // Zwischenspeicher 

    // Ermittlung der CuttingDistance. Kleinste Face des Meshes durch 2.

   // double cuttingDistance = 0.1;
      
    double cuttingDistance = 10000;
    

    for (Mesh::Face_index fi : diffGeometry.faces()) {
        CGAL::Bbox_3 fi_test = CGAL::Polygon_mesh_processing::face_bbox(fi, diffGeometry);
        double abs = std::abs(fi_test.zmax() - fi_test.zmin());
        if (abs < cuttingDistance && abs >0) {
            cuttingDistance = abs;
        };
    };
    cuttingDistance = cuttingDistance / 2;
    double z = zMaxDiffGeometry + cuttingDistance; // Startwert um cuttingDistance versetzt

    while (-z >= zMinDiffGeometry) {
        slicer(K::Plane_3(0, 0, 1, z), std::back_inserter(slicesDiffGeometry));
        std::cout << "At z = " << z << ", the slicer intersects " << slicesDiffGeometry.size() << " polylines" << std::endl;

        //Polylines in Polygon_2 überführen
        std::vector<Polygon_2> polygonList; // Zwischenspeicher für PolygonContainer
        for (const auto& result : slicesDiffGeometry) {

            for (const auto& res : result) {
                p.push_back(Point(res.x(), res.y()));
            }
            polygonList.push_back(p);
            p.clear();
        };
        slicesDiffGeometryPolygon.insert(std::pair<std::string, std::vector<Polygon_2>>(std::to_string(-z), polygonList));
        if (-z == zMinDiffGeometry) {
            break;
        };
        z = z + cuttingDistance;
        if (-z < zMinDiffGeometry) {
            // Bodengeometrie
            z = -zMinDiffGeometry;
        };
        slicesDiffGeometry.clear();
   
    }
    return slicesDiffGeometryPolygon;
};