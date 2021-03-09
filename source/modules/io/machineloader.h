#pragma once
#include <iostream>
#include <filesystem>
#include <algorithm>
#include "millingmachine.h"
#include "config.h"
std::vector<MillingMachine> loadMachines(Config &config);

bool isMillingMachine(std::filesystem::path);