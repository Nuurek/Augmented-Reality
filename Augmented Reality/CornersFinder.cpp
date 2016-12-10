#include "CornersFinder.h"


CornersFinder::CornersFinder(const size_t borderSize, const size_t regionSize, const size_t stepSize) :
	LineSegmentMerger(borderSize, regionSize, stepSize) {
}

std::vector<LineSegment> CornersFinder::findLinesWithCorners(std::vector<LineSegment> lineSegments) {
	std::vector<LineSegment> lineSegmentsWithCorner;

	for (auto& lineSegment : lineSegments) {
		if (isLineSegmentWithBorder(lineSegment)) {
			lineSegmentsWithCorner.emplace_back(LineSegment(lineSegment));
		}
	}

	return lineSegmentsWithCorner;
}


bool CornersFinder::isLineSegmentWithBorder(LineSegment& lineSegment) const {
	const auto dx = static_cast<int>(lineSegment.slope.x * DELTA_MULTIPLIER);
	const auto dy = static_cast<int>(lineSegment.slope.y * DELTA_MULTIPLIER);

	auto x = lineSegment.start.position.x - dx;
	auto y = lineSegment.start.position.y - dy;

	if (isPixelWhite(x, y)) {
		lineSegment.startCorner = true;
	}
	
	x = lineSegment.end.position.x + dx;
	y = lineSegment.end.position.y + dy;

	if (isPixelWhite(x, y)) {
		lineSegment.endCorner = true;
	}

	return lineSegment.startCorner || lineSegment.endCorner;
}