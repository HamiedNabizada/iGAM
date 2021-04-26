#include "rawMaterialSelection.h"

RawMaterial selectRawMaterial(std::vector<RawMaterial> rawMaterials, Mesh inputGeometry)
{
	Mesh convexHull;
	CGAL::convex_hull_3(inputGeometry.points().begin(), inputGeometry.points().end(), convexHull);
	CGAL::Bbox_3 bb_cv = CGAL::Polygon_mesh_processing::bbox_3(convexHull);
	double x_length = bb_cv.xmax() - bb_cv.xmin();
	double y_length = bb_cv.ymax() - bb_cv.ymin();
	double z_length = bb_cv.zmax() - bb_cv.zmin();
	std::vector<RawMaterial> suitableRawMaterials; // Alle Rohteile, in denen Werkstück hineinpasst.

	for (RawMaterial rm : rawMaterials)
	{
		std::vector<double> dimensions = { x_length, y_length, z_length };
		std::sort(dimensions.begin(), dimensions.end());

		double height_rm = rm.height[1] - rm.height[0];
		double width_rm = rm.width[1] - rm.width[0];
		double length_rm = rm.height[1] - rm.height[0];
		std::vector<double> dimensions_rm = { height_rm, width_rm, length_rm };
		std::sort(dimensions_rm.begin(), dimensions_rm.end());
		bool valid = true;
		std::vector<std::tuple<double, double>> result;
		// Prüfen ob Werkstück in Rohteil hineinpasst.
		while (!dimensions_rm.empty())
		{
			double tmp_rm = dimensions_rm.back();
			dimensions_rm.pop_back();
			double tmp_ch = dimensions.back();
			dimensions.pop_back();
			tmp_rm >= tmp_ch ? result.push_back(std::make_tuple(tmp_ch, tmp_rm)) : valid = false;
		};
		if (valid)
		{
			std::cout << "Raw material with id: " << rm.id << " is suitable" << std::endl;
			double x;
			double y;
			double z;
			for (const auto& p : result)
			{
				if (std::get<0>(p) == x_length)
				{
					x = std::get<1>(p);
				};
				if (std::get<0>(p) == y_length)
				{
					y = std::get<1>(p);
				};
				if (std::get<0>(p) == z_length)
				{
					z = std::get<1>(p);
				}
			};
			// u            q
			// +------------+
			// |            |
			// |            |
			// |      f     |
			// |            |
			// |            |
			// +------------+
			// v            w
			std::vector<Point_3> points;
			points.push_back(Point_3(0, 0, 0)); //u
			points.push_back(Point_3(0, y, 0)); //v
			points.push_back(Point_3(x, y, 0)); //w
			points.push_back(Point_3(x, 0, 0)); //q
			points.push_back(Point_3(x, 0, z)); //q'
			points.push_back(Point_3(x, y, z)); //w'
			points.push_back(Point_3(0, y, z)); //v'
			points.push_back(Point_3(0, 0, z)); //u'
			CGAL::convex_hull_3(points.begin(), points.end(), rm.geometry);
			rm.volume = CGAL::Polygon_mesh_processing::volume(rm.geometry).exact().to_double();
			suitableRawMaterials.push_back(rm);
		}
		else
		{
			std::cout << "Raw material with id: " << rm.id << " is not suitable" << std::endl;
		};
	};
	RawMaterial selectedRawMaterial = suitableRawMaterials.at(0);

	for (RawMaterial rm : suitableRawMaterials)
	{
		if (rm.volume < selectedRawMaterial.volume)
		{
			selectedRawMaterial = rm;
		};
	};

	double convexHullVolume = CGAL::Polygon_mesh_processing::volume(convexHull).exact().to_double();
	std::cout << "_______Best fitting raw material____________________" << std::endl;
	std::cout << "id: " << selectedRawMaterial.id << "" << std::endl;
	std::cout << "oversized volume: " << selectedRawMaterial.volume - convexHullVolume << std::endl;
	std::cout << "____________________________________________________" << std::endl;
	return selectedRawMaterial;
};