#include "machineloader.h"

std::vector<MillingMachine> loadMachines()
{
	std::filesystem::path path = std::filesystem::current_path();
	path /= "data";
	path /= "machines";

	std::vector<MillingMachine> result;

	for (const auto& p : std::filesystem::recursive_directory_iterator(path))
	{
		if(p.path().filename().extension() == ".json")
		{
			if(isMillingMachine(p))
			{
				MillingMachine mm = MillingMachine(p.path(), true);
				result.push_back(mm);
			}		
		}
	};
	return result;
};
bool isMillingMachine(std::filesystem::path p)
{
	std::filesystem::path mm = "millingmachines";
	return std::search(p.begin(), p.end(), mm.begin(), mm.end()) != p.end();

};