#pragma once
#include <unordered_map>
#include <string>


class KeyManager {
	std::unordered_map<std::string, bool> options;
	std::unordered_map<char, std::string> keyToOptionsMap{
		{'1', "regions"},
		{'2', "subRegions"},
		{'3', "edgels"},
		{'4', "lineSegments"},
		{'5', "mergedLineSegments"},
		{'6', "extendedLineSegments"},
		{'7', "lineSegmentsWithCorner"}
	};

public:
	void keyPressed(char keyCode);
	bool isActive(std::string option);
};

