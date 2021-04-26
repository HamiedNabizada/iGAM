#include "removableVolume.h"
// new
std::vector<Mesh> computeRemovableVolumes(std::map<std::string, std::vector<OffsetContainer>> offsets, Mesh workingPieceGeometry, MillingTool millingTool, Mesh convexHull, Config& config, std::filesystem::path out) {

	std::filesystem::path outputPlanes = out / "planes";
	CGAL::Bbox_3 bboxCH = CGAL::Polygon_mesh_processing::bbox(convexHull);
	const double zMin = bboxCH.zmin();
	std::ofstream debuger;
	debuger.open("output.txt");
	if (config.getSavePlanes()) std::filesystem::create_directories(outputPlanes);
	
	Tree workingPieceTree(faces(workingPieceGeometry).first, // Transformieren zu AABB tree für Intersection Computation
						  faces(workingPieceGeometry).second, 
						  workingPieceGeometry);

	std::map<double, Mesh> result; // Zwischenspeicher für Planes
	std::vector<Mesh> output;      // Rückgabewert
	debuger << "Starting Analysis" << std::endl;

	std::cout << "Computing removable Volumes" << std::endl;
	CGAL::Timer().start();
	// Ermitteln der Z-Koordinate -> Auf Prüfpunkt wird Ray geschossen und Intersection Punkt ermittelt (Sichtbarkeitsanalyse)
	for (std::pair<std::string, std::vector<OffsetContainer>> offset_polygons : offsets) {
		debuger << "z: " <<offset_polygons.first <<"---------------------------------------------------------" << std::endl;
		double z = std::stod(offset_polygons.first);
		for each (OffsetContainer oc in offset_polygons.second) {
			for (int i = 0; i < oc.size(); ++i) {
				std::vector<Point_3> points; // Speicher für zulässige Punkte
				bool changeZpositions = false;
				double validZ = zMin; 
				for (std::vector<Point_3>::const_iterator it = oc[i].begin(); it != oc[i].end(); ++it) {
					//Prüfpunkte
					Point_3 xyzcord = *it;
					double x = xyzcord.x().exact().to_double(); 
					double y = xyzcord.y().exact().to_double();
					debuger << "checkpoint: " << x << " " << y << " " << z << std::endl;

					Ray ray(Point_3(x, y, 2), Point_3(x, y, 0)); // Strahl orthogonal zum Prüfpunkt 
					Ray_intersection intersection = workingPieceTree.first_intersection(ray); 
					double z_intersection;
					if (intersection) {
						if (boost::get<Point_3>(&(intersection->first))) {
							const Point_3* p = boost::get<Point_3>(&(intersection->first));
							z_intersection = p->exact().z().to_double(); // Z-Koordinate auf die Strahl getroffen hat
							if (z_intersection <= z) {  // Punkt nur aufnehmen, wenn er sichtbar ist
								debuger << "Point is visible. Intersection z: " << z_intersection << " validZ: " << validZ << std::endl;
								Point_3 checkPoint = Point_3(x, y, z_intersection);
								// Kollisionsdetektion, Punkt wird nur aufgenommen, wenn er nicht in die Werkzeuggeomtrie schneidet (Insgesamt werden zwei Versuche unternommen einen neuen Punkt zu finden) 
								//if (checkPosition(debuger, workingPieceGeometry, millingTool.toolGeometry, millingTool.diameter, checkPoint, config.getSaveInterstions(), out)) {
								if(true){
									// Prüfen ob alle Punkte auf einer Ebene liegen:
									// Fall1: Es sind noch keine Punkte aufgenommen worden
									if (points.empty()) {
										validZ = z_intersection;
									}
									// Fall2: Z-Koordinaten zuvor aufgenommener Punkte sind größer als Z-Koordinate des aktuellen checkPoints -> Ändern auf Z-Koordinate der bereits aufgenommenen Punkte 
									else if (validZ > z_intersection) {
										checkPoint = Point_3(checkPoint.x().exact().to_double(), checkPoint.y().exact().to_double(), validZ);
									}
									// Fall3: Z-Koordinaten zubvor aufgenommern Punkte sind tiefer als Z-Koordinate des aktuellen checkPoints -> Alle Z-Koordinaten der bereits aufgenommen Punkte auf diese Z-Koordinate ändern
									else if (validZ < z_intersection) {
										validZ = z_intersection;
										setNewZPosition(points, validZ);
										for each (Point_3 p in points) {
											debuger << "newEnd: " << p.x().exact().to_double() << " " << p.y().exact().to_double() << " " << p.z().exact().to_double() << std::endl;
										};
									};
									points.push_back(checkPoint);
								};
							};
							/*
							std::cout << "z_intersection: " << z_intersection << std::endl;
							Point_3 checkPoint = Point_3(x, y, z_intersection);
							Mesh toolTemp = positioningMillingTool(millingTool.toolGeometry, Point(x, y), z_intersection, config.getSaveToolPositions(), out);
							checkPosition(workingPieceGeometry, toolTemp, checkPoint, config.getSaveInterstions(), out);

							points.push_back(checkPoint);
							if (z_intersection > validZ) validZ = z_intersection; // Damit Z-Position für ganze Ebene gleich ist
							//if (z < z_intersection) valid = false; // Falls z-Position nicht sichtbar, nicht aufnehmen
							*/
						}
					}
				}

				// Erstellen der abtragbaren Volumengeometrie
				if (!points.empty()) {
					debuger << "-------------------- " << std::endl;
					debuger << "pointsBMS: " << std::endl;
					for each (Point_3 p in points) {
						debuger << p.x().exact().to_double() << " " << p.y().exact().to_double() << " " << p.z().exact().to_double() << std::endl;
					};
					typedef CGAL::Triple<int, int, int> Triangle_int;
					std::vector<Triangle_int> patch;
					patch.reserve(points.size() - 2); // there will be exactly n-2 triangles in the patch
					CGAL::Polygon_mesh_processing::triangulate_hole_polyline(points, std::back_inserter(patch));

					Mesh sm;
					for (std::size_t i = 0; i < patch.size(); ++i)
					{
						Mesh::Vertex_index u = sm.add_vertex(points[patch[i].first]);
						Mesh::Vertex_index v = sm.add_vertex(points[patch[i].second]);
						Mesh::Vertex_index w = sm.add_vertex(points[patch[i].third]);
						Mesh::Face_index fi = sm.add_face(u, v, w);
					};
					if (!sm.is_valid()) debuger << " Plane is invalid " << std::endl;
					if (sm.is_valid()) debuger << " Plane is valid " << std::endl;
					//if (sm.is_valid()) {
						//std::cout << "surfaceMesh is valid " << std::endl;
						CGAL::Bbox_3 bboxSm = CGAL::Polygon_mesh_processing::bbox(sm);
						double x_length = bboxSm.xmax() - bboxSm.xmin();
						double y_length = bboxSm.ymax() - bboxSm.ymin();
						double area = CGAL::Polygon_mesh_processing::area(sm).exact().to_double();
						double res_key = bboxSm.xmax() + bboxSm.xmin() + bboxSm.ymax() + bboxSm.ymin() + bboxSm.zmax() + bboxSm.zmin();
						if (result.find(res_key) == result.end()) {
							result.insert(std::pair<double, Mesh>(res_key, sm));
							if (config.getSavePlanes()) {
								std::string filename = "plane_" + std::to_string(i) + "_z_" + std::to_string(z) + "_(" + std::to_string(res_key) + "_" + std::to_string(std::rand()) + ").off";
								debuger << filename << std::endl;
								std::ofstream outstream(outputPlanes / filename);
								CGAL::write_off(outstream, sm);
								outstream.close();
							}
							//output.push_back(computeMinkowskiSum(sm, millingTool.toolGeometry, config, out));
							std::pair<bool, Mesh> rv = computeMinkowskiSum(debuger,points, millingTool, config, out);
							if (rv.first) output.push_back(rv.second);
						};
					
				};

			};

		}
	};
	debuger.close();
	CGAL::Timer().stop();
	std::cout << "-> Computing removable Volumes finished (" << CGAL::Timer().time() << ")" << std::endl;
	return output;
};

std::pair<bool, Mesh> computeMinkowskiSum(std::ofstream& debuger, std::vector<Point_3> points, MillingTool millingTool, Config& config, std::filesystem::path out) {
	
	debuger << "-------------------- " << std::endl;
	debuger << "computeMinkowskiSum" << std::endl;
	for each (Point_3 p in points) {
		debuger << p.x().exact().to_double() << " " << p.y().exact().to_double() << " " << p.z().exact().to_double() << std::endl;
	};
	Nef_polyhedron planeNef(points.begin(), points.end());
	Nef_polyhedron toolNef(millingTool.toolGeometry);
	Nef_polyhedron result = CGAL::minkowski_sum_3(planeNef, toolNef);
	Mesh output;
	CGAL::convert_nef_polyhedron_to_polygon_mesh(result, output);
	CGAL::Bbox_3 bbO = CGAL::Polygon_mesh_processing::bbox(output);
	double sideX = bbO.xmax() - bbO.xmin();
	double sideY = bbO.ymax() - bbO.ymin();
	if (config.getSaveRemovableVolumes()) {
		std::filesystem::path outputPath = out / "removableVolumes";

		std::filesystem::create_directories(outputPath);
		std::string fileName = "RemovableVolume" + std::to_string(rand()) + ".off";
		std::ofstream outstream(outputPath / fileName);
		CGAL::write_off(outstream, output);
		outstream.close();
		debuger << "filename: " << fileName << std::endl;
	};
	return std::pair<bool, Mesh>(true, output);
};

// OLD
Mesh computeMinkowskiSum(Mesh plane, Mesh toolGeometry, Config& config, std::filesystem::path out) {
	Nef_polyhedron planeNef(plane);
	Nef_polyhedron toolNef(toolGeometry);
	Nef_polyhedron result = CGAL::minkowski_sum_3(planeNef, toolNef);
	Mesh output;
	CGAL::convert_nef_polyhedron_to_polygon_mesh(result, output);
	if (config.getSaveRemovableVolumes()) {
		std::filesystem::path outputPath = out;
		outputPath /= "removableVolumes";
		std::filesystem::create_directories(outputPath);
		std::string fileName = "minkowskisum" + std::to_string(rand()) + ".off";
		std::ofstream outstream;
		outstream.open(outputPath / fileName);
		outstream << output;
		outstream.close();
	};
	return output;
};

Mesh computeLeftover(std::vector<Mesh> minkowskiSums, Mesh convexHull, Config& config, std::filesystem::path out) {
	Nef_polyhedron convexHullNef(convexHull);
	// TODO: Unnötig hin und her zutransformieren, besser minkowskiSummen gleich als Nef_polyhedron beibehalten (Update: Vlt doch sinnvoll zunächst in Mesh zu transformieren um Simplification Algorithmus anwenden zu können)
	// Test: Erst Unions berechnen, danach Differenz
	std::ofstream ost;
	std::cout << "Computing Endgeometry" << std::endl;
	CGAL::Timer().start();
	CGAL::Nef_nary_union_3<Nef_polyhedron> nary;
	for (int i = 0; i < minkowskiSums.size(); ++i) {
		std::cout << "     Removable Volume " << i << " of " << minkowskiSums.size() << std::endl;
		Nef_polyhedron msNef(minkowskiSums[i]);
		nary.add_polyhedron(msNef);
	};
	Nef_polyhedron unionresult = convexHullNef - nary.get_union();
	Mesh output;
	CGAL::convert_nef_polyhedron_to_polygon_mesh(unionresult, output);
	std::string filename = "FinalGeometry.off";
	std::ofstream outstream(out /filename);
	CGAL::write_off(outstream, output);
	outstream.close();
	CGAL::Timer().stop();
	std::cout << "-> Computing Endgeomerty finished (" << CGAL::Timer().time() << ")" << std::endl;
	CGAL::Timer().reset();



	return output;
};


void setNewZPosition(std::vector<Point_3>& points, double zpos) {
	std::vector<Point_3> result;
	for each (Point_3 p in points) {
		std::cout << "old: " << p.x().exact().to_double() << " " << p.y().exact().to_double() << " " << p.z().exact().to_double() << std::endl;
		result.push_back(Point_3(p.x().exact().to_double(), p.y().exact().to_double(), zpos));
	};
	for each (Point_3 p in result) {
		std::cout << "new: " << p.x().exact().to_double() << " " << p.y().exact().to_double() << " " << p.z().exact().to_double() << std::endl;
	};
	points.clear();
	points = result;
	return;
};