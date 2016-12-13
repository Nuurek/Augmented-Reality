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

	mergedLineSegments = lineSegmentMerger.mergeLineSegments(mergedLineSegments);

	extendedLineSegments = lineSegmentExtender.extendLineSegments(mergedLineSegments);

	lineSegmentsWithCorner = cornersFinder.findLinesWithCorners(extendedLineSegments);

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