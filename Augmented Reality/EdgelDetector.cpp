#include "EdgelDetector.h"


void EdgelDetector::setBuffer(Buffer* buffer) {
	this->buffer = buffer;
}

std::vector<Edgel> EdgelDetector::findEdgels() {
	size_t width = buffer->getWidth();
	size_t height = buffer->getHeight();

	size_t maxWidth = width - BORDER_SIZE - 1;
	size_t maxHeight = height - BORDER_SIZE - 1;

	long long totalTime = 0;

	for (size_t regionTop = BORDER_SIZE; regionTop < maxHeight; regionTop += REGION_SIZE) {
		for (size_t regionLeft = BORDER_SIZE; regionLeft < maxWidth; regionLeft += REGION_SIZE) {
			size_t regionRight = std::min(regionLeft + REGION_SIZE, maxWidth);
			size_t regionBottom = std::min(regionTop + REGION_SIZE, maxHeight);

			auto start = std::chrono::high_resolution_clock::now();
			auto edgels = findEdgelsInRegion(regionLeft, regionTop, regionRight, regionBottom);
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			// std::cout << "One region: " << duration << "us.\n";
			totalTime += duration;
		}
	}

	std::cout << "Calculating regions: " << totalTime << "us.\n";

	return std::vector<Edgel>();
}

std::vector<Edgel> EdgelDetector::findEdgelsInRegion(size_t regionLeft, size_t regionTop, size_t regionRight, size_t regionBottom) {
	size_t width = buffer->getWidth();

	unsigned int previousPixelOne, previousPixelTwo;
	std::vector<Edgel> edgels;

	for (size_t y = regionTop; y < regionBottom; y += STEP_SIZE) {
		unsigned char* startPosition = buffer->getData() + (regionLeft + y * width) * NUMBER_OF_CHANNELS;
		previousPixelOne = previousPixelTwo = 0;
		const size_t pitch = NUMBER_OF_CHANNELS;

		for (size_t x = regionLeft; x < regionRight; x++) {
			unsigned char* pixelPosition = startPosition + x * pitch;
			unsigned int firstChannel = edgeKernel(pixelPosition, pitch);
			if (firstChannel > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 1, pitch) > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 2, pitch) > KERNEL_THRESHOLD) {
			} else {
				firstChannel = 0;
			}

			if (previousPixelOne > 0 && previousPixelOne > previousPixelTwo && previousPixelOne > firstChannel) {


			}
			previousPixelTwo = previousPixelOne;
			previousPixelOne = firstChannel;
		}
	}
	return std::vector<Edgel>();
}

unsigned int EdgelDetector::edgeKernel(unsigned char* offset, size_t pitch) const {
	int kernel = 0;

	for (size_t index = 0; index < 2 * BORDER_SIZE + 1; index++) {
		kernel += GAUSSIAN_COEFFICIENTS[index] * offset[(index - BORDER_SIZE) * pitch];
	}

	return abs(kernel);
}
