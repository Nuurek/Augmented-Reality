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
