#pragma once
#include "LineSegmentMerger.h"

const float DELTA_MULTIPLIER = 4.0f;
const float WHITE_THRESHOLD = 10.0f;

class CornersFinder :
	public LineSegmentMerger {

	bool isLineSegmentWithBorder(LineSegment& lineSegment) const;
	inline bool isPixelWhite(float x, float y) const {
		return (
			buffer->getPixelChannel(x, y, 0) > WHITE_THRESHOLD &&
			buffer->getPixelChannel(x, y, 1) > WHITE_THRESHOLD &&
			buffer->getPixelChannel(x, y, 2) > WHITE_THRESHOLD
			);
	}

public:
	CornersFinder(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	std::vector<LineSegment> findLinesWithCorners(std::vector<LineSegment> lineSegments);
};

