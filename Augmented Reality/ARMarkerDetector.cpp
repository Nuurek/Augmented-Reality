#include "ARMarkerDetector.h"


ARMarkerDetector::ARMarkerDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize)
	: RegionBasedOperator(borderSize, regionSize, stepSize),
	edgelDetector(borderSize, regionSize, stepSize),
	lineSegmentMerger(borderSize, regionSize, stepSize),
	lineSegmentExtender(borderSize, regionSize, stepSize),
	cornersFinder(borderSize, regionSize, stepSize) {}

void ARMarkerDetector::setBuffer(Buffer * buffer) {
	RegionBasedOperator::setBuffer(buffer);
	edgelDetector.setBuffer(buffer);
	lineSegmentMerger.setBuffer(buffer);
	lineSegmentExtender.setBuffer(buffer);
	cornersFinder.setBuffer(buffer);
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

			std::vector<LineSegment> lineSegmentsInRegion;
			if (edgelsInRegion.size() > EDGELS_IN_REGION) {
				lineSegmentsInRegion = lineSegmentDetector.findLineSegmentsInRegion(edgelsInRegion);
				lineSegments.insert(lineSegments.end(), lineSegmentsInRegion.begin(), lineSegmentsInRegion.end());
			}

			end = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			findingLineSegments += duration;
			//

			//
			/* 
				TODO: Right now line segments are merged firstly in the region and then globally
				what can result in a lot of compares in global phase. 
				We could enlarge merge region iteratively instead.
			*/
			if (lineSegmentsInRegion.size() > 0) {
				auto mergedLineSegmentsInRegion = lineSegmentMerger.mergeLineSegments(lineSegmentsInRegion);
				mergedLineSegments.insert(mergedLineSegments.end(), mergedLineSegmentsInRegion.begin(), mergedLineSegmentsInRegion.end());
			}
			//
		}
	}

	std::cout << "Finding edgels: " << findingEdgelsTime << "us.\n";
	std::cout << "Edgels: " << edgels.size() << "\n";
	std::cout << "Finding line segments: " << findingLineSegments << "us.\n";

	std::cout << "Line segments in regions: " << lineSegments.size() << "\n";
	std::cout << "Merged line segments in regions: " << mergedLineSegments.size() << "\n";

	mergedLineSegments = lineSegmentMerger.mergeLineSegments(mergedLineSegments);

	std::cout << "Merged line segments: " << mergedLineSegments.size() << "\n";

	extendedLineSegments = lineSegmentExtender.extendLineSegments(mergedLineSegments);

	std::cout << "Extended line segments: " << extendedLineSegments.size() << "\n";

	lineSegmentsWithCorner = cornersFinder.findLinesWithCorners(extendedLineSegments);

	std::cout << "Line segments with corner: " << lineSegmentsWithCorner.size() << "\n";

	arMarkers = markerFinder.findMarkers(lineSegmentsWithCorner);
}

std::vector<Edgel> ARMarkerDetector::getEdgels() {
	return edgels;
}

std::vector<LineSegment> ARMarkerDetector::getLineSegments() {
	return lineSegments;
}

std::vector<LineSegment> ARMarkerDetector::getMergedLineSegments() {
	return mergedLineSegments;
}

std::vector<LineSegment> ARMarkerDetector::getExtendedLineSegments() {
	return extendedLineSegments;
}

std::vector<LineSegment> ARMarkerDetector::getLineSegmentsWithCorner() {
	return lineSegmentsWithCorner;
}

std::vector<ARMarker> ARMarkerDetector::getARMarkers() {
	return arMarkers;
}

void ARMarkerDetector::clearStructures() {
	edgels.clear();
	lineSegments.clear();
	mergedLineSegments.clear();
	extendedLineSegments.clear();
	lineSegmentsWithCorner.clear();
	arMarkers.clear();
}