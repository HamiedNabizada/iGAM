#include "rawMaterialLoader.h"


std::vector<RawMaterial> loadRawMaterial(Config &config)
{
	std::filesystem::path path = std::filesystem::current_path();
	path /= "data";
	path /= "rawmaterial";
	
	std::vector<RawMaterial> result;
	
	for (const auto& p : std::filesystem::directory_iterator(path))
	{
		if(p.path().filename().extension() == ".json")
		{
			std::ifstream filestream(p.path().c_str());
			json jsonData;
			filestream >> jsonData;
			filestream.close();
			RawMaterial rm(jsonData, config.getPrintRawMaterialInformation());
			result.push_back(rm);
		}	
	};
	return result;
};
