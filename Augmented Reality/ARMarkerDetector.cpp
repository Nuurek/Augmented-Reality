#include "ARMarkerDetector.h"


ARMarkerDetector::ARMarkerDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize)
	: RegionBasedOperator(borderSize, regionSize, stepSize),
	edgelDetector(borderSize, regionSize, stepSize) {}

void ARMarkerDetector::setBuffer(Buffer * buffer) {
	RegionBasedOperator::setBuffer(buffer);
	edgelDetector.setBuffer(buffer);
}

void ARMarkerDetector::findARMarkers() {
	size_t width = buffer->getWidth();
	size_t height = buffer->getHeight();

	size_t maxWidth = width - BORDER_SIZE - 1;
	size_t maxHeight = height - BORDER_SIZE - 1;

	long long findingEdgelsTime = 0, findingLineSegments = 0;

	clearStructures();

	for (size_t regionTop = BORDER_SIZE; regionTop < maxHeight; regionTop += REGION_SIZE) {
		for (size_t regionLeft = BORDER_SIZE; regionLeft < maxWidth; regionLeft += REGION_SIZE) {
			size_t regionWidth = std::min(REGION_SIZE, maxWidth - regionLeft);
			size_t regionHeight = std::min(REGION_SIZE, maxHeight - regionTop);

			//
			auto start = std::chrono::high_resolution_clock::now();

			auto edgelsInRegion = edgelDetector.findEdgelsInRegion(regionLeft, regionTop, regionWidth, regionHeight);
			edgels.insert(edgels.end(), edgelsInRegion.begin(), edgelsInRegion.end());

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			findingEdgelsTime += duration;
			//

			//
			start = std::chrono::high_resolution_clock::now();

			if (edgelsInRegion.size() > EDGELS_IN_REGION) {
				auto lineSegmentsInRegion = lineSegmentDetector.findLineSegmentsInRegion(edgelsInRegion);
				lineSegments.insert(lineSegments.end(), lineSegmentsInRegion.begin(), lineSegmentsInRegion.end());
			}

			end = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			findingLineSegments += duration;
			//
		}
	}

	std::cout << "Finding edgels: " << findingEdgelsTime << "us.\n";
	std::cout << "Finding line segments: " << findingLineSegments << "us.\n";
}

std::vector<Edgel> ARMarkerDetector::getEdgels() {
	return edgels;
}

std::vector<LineSegment> ARMarkerDetector::getLineSegments() {
	return lineSegments;
}

void ARMarkerDetector::clearStructures() {
	edgels.clear();
	lineSegments.clear();
}