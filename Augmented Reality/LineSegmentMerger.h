#pragma once

#include <utility>

#include "RegionBasedOperator.h"
#include "EdgelDetector.h"
#include "LineSegment.h"


const float COMMON_ORIENTATION_THRESHOLD = 0.99f;
const float LENGTH_THRESHOLD = 25.0f;
const float SQUARED_LENGTH_THRESHOLD = LENGTH_THRESHOLD * LENGTH_THRESHOLD;
const size_t MERGE_ITERATIONS = 50;

struct LineWithDistance {
	LineSegment& lineSegment;
	float distance;

	LineWithDistance(LineSegment& lineSegment, float distance) :
		lineSegment(lineSegment), distance(distance) { }

	inline bool operator<(LineWithDistance& rhs) {
		return distance < rhs.distance;
	}

	inline LineWithDistance& operator=(LineWithDistance& rhs) {
		lineSegment = rhs.lineSegment;
		distance = rhs.distance;
		return *this;
	}
};

class LineSegmentMerger : public EdgelDetector {
protected:
	bool orientationAndDirectionCompatible(LineSegment& first, LineSegment& second);
	bool isLineExtensible(Vector2f startPoint, Vector2f& endPoint, const Vector2f slope, const Vector2f gradient, const size_t maxLength);
	bool isGradientIntensive(Vector2f startPoint, Vector2f normal, Vector2f gradient) const;

public:
	LineSegmentMerger(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	
	std::vector<LineSegment> mergeLineSegments(std::vector<LineSegment> lineSegments);
};

