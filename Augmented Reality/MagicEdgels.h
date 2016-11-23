#pragma once
#include <vector>
#include <unordered_map>
#include <chrono>

#include "Edgel.h"


struct EdgelComparator {
	size_t operator()(const Edgel& edgel) const {
		return (std::hash<float>()(edgel.position.x * 640 + edgel.position.y));
	}
};

class MagicEdgels {
	double liveInterval = 0;
	std::unordered_map<Edgel, int, EdgelComparator> liveEdgels;

public:
	MagicEdgels(int liveInterval);
	void updateEdgels(std::vector<Edgel>& newEdgels);
	std::vector<Edgel> getLiveEdgels();
	std::vector<std::pair<Edgel, double>> getMagicEdgels();
};

