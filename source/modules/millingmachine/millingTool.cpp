#include "millingTool.h"



void MillingTool::loadMillingTool(std::filesystem::path pathToolFile) {
	
	name = pathToolFile.filename().string();
	toolGeometry = importerOff(pathToolFile, true);
	toolGeometry = centerGeometry(toolGeometry, true);
	bbMillingTool = CGAL::Polygon_mesh_processing::bbox_3(toolGeometry);
	double diameterX = bbMillingTool.xmax() - bbMillingTool.xmin();
	double diameterY = bbMillingTool.ymax() - bbMillingTool.ymin();
	diameter = (diameterX > diameterY) ? diameterX : diameterY;
}
