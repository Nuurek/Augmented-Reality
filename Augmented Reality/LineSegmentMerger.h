#pragma once

#include "LineSegment.h"


const float COMMON_ORIENTATION_THRESHOLD = 0.99f;
const float LENGTH_THRESHOLD = 25.0f;
const float SQUARED_LENGTH_THRESHOLD = LENGTH_THRESHOLD * LENGTH_THRESHOLD;


class LineSegmentMerger {
	bool orientationAndDirectionCompatible(LineSegment& first, LineSegment& second);

public:
	std::vector<LineSegment> mergeLineSegments(std::vector<LineSegment> lineSegmentsInRegion);
};

