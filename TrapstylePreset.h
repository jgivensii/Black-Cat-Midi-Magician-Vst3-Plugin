#pragma once
#include <string>
#include <array>
#include <vector>
#include <utility>

struct TrapstylePreset
{
	std::string name;
	std::array<int, 16> rhythmPattern;
	std::vector<int> intervalBias;
	std::pair<int, int>noteDensityRange;
};

extern std::vector<TrapstylePreset> trapStyles;