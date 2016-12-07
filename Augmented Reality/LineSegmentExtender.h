#pragma once
#include "LineSegmentMerger.h"


const size_t EXTENSION_LENGTH = 1000;
const float DELTA_MULTIPLIER = 4.0f;
const unsigned char WHITE_THRESHOLD = 10;

class LineSegmentExtender : public LineSegmentMerger {
	bool isLineSegmentWithBorder(LineSegment& lineSegment) const;
	inline bool isPixelWhite(size_t x, size_t y) const {
		return (
			buffer->getPixelChannel(x, y, 0) > WHITE_THRESHOLD &&
			buffer->getPixelChannel(x, y, 1) > WHITE_THRESHOLD &&
			buffer->getPixelChannel(x, y, 2) > WHITE_THRESHOLD
			);
	}

public:
	LineSegmentExtender(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	std::vector<LineSegment> extendLineSegments(std::vector<LineSegment> lineSegments);
	std::vector<LineSegment> findLinesWithCorners(std::vector<LineSegment> lineSegments);
};

