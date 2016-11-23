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

	long long totalTime = 0;

	clearStructures();

	for (size_t regionTop = BORDER_SIZE; regionTop < maxHeight; regionTop += REGION_SIZE) {
		for (size_t regionLeft = BORDER_SIZE; regionLeft < maxWidth; regionLeft += REGION_SIZE) {
			size_t regionWidth = std::min(REGION_SIZE, maxWidth - regionLeft);
			size_t regionHeight = std::min(REGION_SIZE, maxHeight - regionTop);

			auto start = std::chrono::high_resolution_clock::now();

			auto edgelsInRegion = edgelDetector.findEdgelsInRegion(regionLeft, regionTop, regionWidth, regionHeight);
			edgels.insert(edgels.end(), edgelsInRegion.begin(), edgelsInRegion.end());

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			totalTime += duration;
		}
	}

	std::cout << "Calculating regions: " << totalTime << "us.\n";
}

std::vector<Edgel> ARMarkerDetector::getEdgels() {
	return edgels;
}

void ARMarkerDetector::clearStructures() {
	edgels.clear();
}