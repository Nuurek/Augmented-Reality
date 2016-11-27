#pragma once

#include <utility>

#include "RegionBasedOperator.h"
#include "EdgelDetector.h"
#include "LineSegment.h"


const float COMMON_ORIENTATION_THRESHOLD = 0.99f;
const float LENGTH_THRESHOLD = 25.0f;
const float SQUARED_LENGTH_THRESHOLD = LENGTH_THRESHOLD * LENGTH_THRESHOLD;
const size_t MERGE_ITERATIONS = 50;


class LineSegmentMerger : public EdgelDetector {
	using IndexedLineDistance = std::tuple<size_t, float>;

	bool orientationAndDirectionCompatible(LineSegment& first, LineSegment& second);

public:
	LineSegmentMerger(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	
	std::vector<LineSegment> mergeLineSegments(std::vector<LineSegment> lineSegmentsInRegion);
};

