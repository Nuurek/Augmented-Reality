#pragma once
#include "Edgel.h"
#include "LineSegment.h"
#include <random>

const size_t SEGMENT_SEARCH_ITERATIONS = 25;
const size_t EDGELS_PAIRING_ITERATIONS = 100;

class LineSegmentDetector{
	std::random_device randomDevice;
	std::mt19937 randomGenerator{ randomDevice() };

	bool isEdgelsPairFound(Edgel& start, Edgel& end, size_t iteration);

public:	
	std::vector<LineSegment> findLineSegmentsInRegion(std::vector<Edgel> edgels);
};
