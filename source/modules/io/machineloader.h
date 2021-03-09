#pragma once
#include <iostream>
#include <filesystem>
#include <algorithm>
//#include <nlohmann/json.hpp>
#include "millingmachine.h"

std::vector<MillingMachine> loadMachines();

bool isMillingMachine(std::filesystem::path);