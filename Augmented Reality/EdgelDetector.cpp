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
	int edgelsFound = 0;

	auto edgels = std::vector<Edgel>();

	for (size_t regionTop = BORDER_SIZE; regionTop < maxHeight; regionTop += REGION_SIZE) {
		for (size_t regionLeft = BORDER_SIZE; regionLeft < maxWidth; regionLeft += REGION_SIZE) {
			size_t regionWidth = std::min(REGION_SIZE, maxWidth - regionLeft);
			size_t regionHeight = std::min(REGION_SIZE, maxHeight - regionTop);

			auto start = std::chrono::high_resolution_clock::now();
			auto edgelsInRegion = findEdgelsInRegion(regionLeft, regionTop, regionWidth, regionHeight);
			//edgels.insert(edgels.end(), edgelsInRegion.begin(), edgelsInRegion.end());
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			totalTime += duration;
			edgelsFound += edgelsInRegion.size();
		}
	}

	std::cout << "Calculating regions: " << totalTime << "us.\n";
	std::cout << "Found edgels: " << edgelsFound << "\n";

	return edgels;
}

std::vector<Edgel> EdgelDetector::findEdgelsInRegion(size_t regionLeft, size_t regionTop, size_t regionWidth, size_t regionHeight) {
	size_t width = buffer->getWidth();

	unsigned int previousPixelOne, previousPixelTwo;
	std::vector<Edgel> edgels;

	for (size_t y = 0; y < regionHeight; y += STEP_SIZE) {
		unsigned char* pixelPosition = buffer->getData() + (regionLeft + (y + regionTop) * width) * NUMBER_OF_CHANNELS;
		previousPixelOne = previousPixelTwo = 0;
		const size_t pitch = NUMBER_OF_CHANNELS;

		for (size_t x = 0; x < regionWidth; x++) {
			unsigned int firstChannel = edgeKernel(pixelPosition, pitch);
			if (firstChannel > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 1, pitch) > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 2, pitch) > KERNEL_THRESHOLD) {
			} else {
				firstChannel = 0;
			}

			if (previousPixelOne > 0 && previousPixelOne > previousPixelTwo && previousPixelOne > firstChannel) {
				edgels.emplace_back(Edgel(x, y));
			}
			previousPixelTwo = previousPixelOne;
			previousPixelOne = firstChannel;

			pixelPosition += pitch;
		}
	}

	for (size_t x = 0; x < regionWidth; x += STEP_SIZE) {
		unsigned char* pixelPosition = buffer->getData() + (regionLeft + x + (regionTop * width)) * NUMBER_OF_CHANNELS;
		previousPixelOne = previousPixelTwo = 0;
		const size_t pitch = width * NUMBER_OF_CHANNELS;

		for (size_t y = 0; y < regionHeight; y++) {
			unsigned int firstChannel = edgeKernel(pixelPosition, pitch);
			if (firstChannel > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 1, pitch) > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 2, pitch) > KERNEL_THRESHOLD) {
			} else {
				firstChannel = 0;
			}

			if (previousPixelOne > 0 && previousPixelOne > previousPixelTwo && previousPixelOne > firstChannel) {
				edgels.emplace_back(Edgel(x, y));
			}
			previousPixelTwo = previousPixelOne;
			previousPixelOne = firstChannel;

			pixelPosition += pitch;
		}
	}

	return edgels;
}

unsigned int EdgelDetector::edgeKernel(unsigned char* offset, size_t pitch) const {
	int kernel = 0;

	for (size_t index = 0; index < 2 * BORDER_SIZE + 1; index++) {
		kernel += GAUSSIAN_COEFFICIENTS[index] * offset[(index - BORDER_SIZE) * pitch];
	}

	return abs(kernel);
}
