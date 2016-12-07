#pragma once
#include "LineSegmentMerger.h"


const size_t EXTENSION_LENGTH = 1000;

class LineSegmentExtender :
	public LineSegmentMerger {

public:
	LineSegmentExtender(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	std::vector<LineSegment> extendLineSegments(std::vector<LineSegment> lineSegments);
};

