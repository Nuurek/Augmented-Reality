#include "EdgelDetector.h"


void EdgelDetector::setBuffer(Buffer* buffer) {
	this->buffer = buffer;
}

std::vector<Edgel> EdgelDetector::findEdgels() {
	size_t width = buffer->getWidth();
	size_t height = buffer->getHeight();


	for (size_t regionTop = BORDER_SIZE; regionTop < height - BORDER_SIZE - 1; regionTop += REGION_SIZE) {
		for (size_t regionLeft = BORDER_SIZE; regionTop < width - BORDER_SIZE - 1; regionTop += REGION_SIZE) {
			size_t borderLeft = regionLeft - BORDER_SIZE;
			size_t borderTop = regionTop - BORDER_SIZE;
			size_t borderRight = std::min(regionLeft + REGION_SIZE + BORDER_SIZE, width);
			size_t borderBottom = std::min(regionTop + REGION_SIZE + BORDER_SIZE, height);
			auto edgelsInRegion = findEdgelsInRegion(borderLeft, borderTop, borderRight, borderBottom);
		}
	}

	return std::vector<Edgel>();
}

std::vector<Edgel> EdgelDetector::findEdgelsInRegion(size_t borderLeft, size_t borderTop, size_t borderRight, size_t borderBottom) {


	return std::vector<Edgel>();
}