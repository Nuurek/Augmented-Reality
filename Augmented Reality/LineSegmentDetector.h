#pragma once
#include "Edgel.h"
#include "LineSegment.h"
#include <random>
#include <limits>

const size_t SEGMENT_SEARCH_ITERATIONS = 25;
const size_t EDGELS_PAIRING_ITERATIONS = 100;
const size_t MIN_EDGELS_PER_LINE = 5;

class LineSegmentDetector{
	std::random_device randomDevice;
	std::mt19937 randomGenerator{ randomDevice() };

	bool isEdgelsPairFound(Edgel& start, Edgel& end, size_t iteration);

public:	
	std::vector<LineSegment> findLineSegmentsInRegion(std::vector<Edgel> edgels);
};
