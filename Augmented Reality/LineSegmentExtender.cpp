#include "LineSegmentExtender.h"


LineSegmentExtender::LineSegmentExtender(const size_t borderSize, const size_t regionSize, const size_t stepSize) :
	LineSegmentMerger(borderSize, regionSize, stepSize) {}

std::vector<LineSegment> LineSegmentExtender::extendLineSegments(std::vector<LineSegment> lineSegments) {
	for (auto& lineSegment : lineSegments) {
		isLineExtensible(lineSegment.end.position, lineSegment.end.position, lineSegment.slope, lineSegment.end.slope, EXTENSION_LENGTH);
		isLineExtensible(lineSegment.start.position, lineSegment.start.position, -lineSegment.slope, lineSegment.end.slope, EXTENSION_LENGTH);
	}

	return lineSegments;
}

std::vector<LineSegment> LineSegmentExtender::findLinesWithCorners(std::vector<LineSegment> lineSegments) {
	std::vector<LineSegment> lineSegmentsWithCorner;

	for (auto& lineSegment : lineSegments) {
		if (isLineSegmentWithBorder(lineSegment)) {
			lineSegmentsWithCorner.emplace_back(LineSegment(lineSegment));
		}
	}

	return lineSegmentsWithCorner;
}


bool LineSegmentExtender::isLineSegmentWithBorder(LineSegment& lineSegment) const {
	bool isStartWhite = false;
	bool isEndWhite = false;
	
	const size_t dx = static_cast<size_t>(lineSegment.slope.x * DELTA_MULTIPLIER);
	const size_t dy = static_cast<size_t>(lineSegment.slope.y * DELTA_MULTIPLIER);

	size_t x = lineSegment.start.position.x - dx;
	size_t y = lineSegment.start.position.y - dy;

	if (isPointInsideFrame(x, y)) {
		if (isPixelWhite(x, y)) {
			lineSegment.startCorner = true;
		}
	}

	x = lineSegment.end.position.x + dx;
	y = lineSegment.end.position.y + dy;

	if (isPointInsideFrame(x, y)) {
		if (!isPixelWhite(x, y)) {
			lineSegment.endCorner = true;
		}
	}

	return lineSegment.startCorner || lineSegment.endCorner;
}