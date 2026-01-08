#pragma once
#include <string>
#include <array>
#include <vector>
#include <utility>

struct ScaleManager
{
	std::string scaleName;
	int rootMidiValue;
};

extern std::vector<ScaleManager> scales;