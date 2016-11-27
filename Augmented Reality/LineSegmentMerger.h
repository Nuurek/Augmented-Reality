#pragma once

#include <utility>

#include "RegionBasedOperator.h"
#include "EdgelDetector.h"
#include "LineSegment.h"


const float COMMON_ORIENTATION_THRESHOLD = 0.99f;
const float LENGTH_THRESHOLD = 25.0f;
const float SQUARED_LENGTH_THRESHOLD = LENGTH_THRESHOLD * LENGTH_THRESHOLD;
const size_t MERGE_ITERATIONS = 50;

struct IndexedLineDistance {
	size_t index;
	float distance;

	IndexedLineDistance(size_t index, float distance) :
		index(index), distance(distance) { }

	inline bool operator<(IndexedLineDistance& rhs) {
		return distance < rhs.distance;
	}
};

class LineSegmentMerger : public EdgelDetector {
	bool orientationAndDirectionCompatible(LineSegment& first, LineSegment& second);

public:
	LineSegmentMerger(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	
	std::vector<LineSegment> mergeLineSegments(std::vector<LineSegment> lineSegmentsInRegion);
};

