#pragma once

#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "rawMaterial.h"
#include "config.h"
using json = nlohmann::json;

std::vector<RawMaterial> loadRawMaterial(Config &config);