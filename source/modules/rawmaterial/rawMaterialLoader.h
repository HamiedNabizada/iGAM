#pragma once

#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "rawMaterial.h"
using json = nlohmann::json;

std::vector<RawMaterial> loadRawMaterial();