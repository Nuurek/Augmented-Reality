#include "EdgelDetector.h"


void EdgelDetector::setBuffer(Buffer* buffer) {
	this->buffer = buffer;
}

std::vector<Edgel> EdgelDetector::findEdgels() {
	size_t width = buffer->getWidth();
	size_t height = buffer->getHeight();

	size_t maxWidth = width - BORDER_SIZE - 1;
	size_t maxHeight = height - BORDER_SIZE - 1;

	for (size_t regionTop = BORDER_SIZE; regionTop < maxHeight; regionTop += REGION_SIZE) {
		for (size_t regionLeft = BORDER_SIZE; regionTop < maxWidth; regionTop += REGION_SIZE) {
			size_t regionRight = std::min(regionLeft + REGION_SIZE, maxHeight);
			size_t regionBottom = std::min(regionTop + REGION_SIZE, maxWidth);
			auto edgels = findEdgelsInRegion(regionLeft, regionTop, regionRight, regionBottom);
		}
	}

	return std::vector<Edgel>();
}

std::vector<Edgel> EdgelDetector::findEdgelsInRegion(size_t regionLeft, size_t regionTop, size_t regionRight, size_t regionBottom) {
	size_t width = buffer->getWidth();

	float previousPixelOne, previousPixelTwo;
	std::vector<Edgel> edgels;

	for (int y = regionTop; y < regionBottom; y += STEP_SIZE) {
		unsigned char* startPosition = buffer->getData() + (regionLeft + y * width) * NUMBER_OF_CHANNELS;
		previousPixelOne = previousPixelTwo = 0.0;
		const size_t pitch = NUMBER_OF_CHANNELS;

		for (size_t x = regionLeft; x < regionRight; x++) {
			unsigned int firstChannel = edgeKernel(startPosition, pitch);
			if (firstChannel > KERNEL_THRESHOLD &&
				edgeKernel(startPosition + 1, pitch) > KERNEL_THRESHOLD &&
				edgeKernel(startPosition + 2, pitch) > KERNEL_THRESHOLD) {

			} else {
				firstChannel = 0.0;
			}

			if (previousPixelOne > 0.0 && previousPixelOne > previousPixelTwo && previousPixelOne > firstChannel) {
				std::cout << "Edgel " << x << " x " << y << "\n";
				edgels.emplace_back(Edgel());
			}
		}
	}
	return std::vector<Edgel>();
}

unsigned int EdgelDetector::edgeKernel(unsigned char* offset, size_t pitch) const {
	return 0;
}
