#include "EdgelDetector.h"


EdgelDetector::EdgelDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize) 
	: RegionBasedOperator(borderSize, regionSize, stepSize) {}

std::vector<Edgel> EdgelDetector::findEdgelsInRegion(size_t regionLeft, size_t regionTop, size_t regionWidth, size_t regionHeight) {
	size_t width = buffer->getWidth();

	unsigned int previousPixelOne, previousPixelTwo;
	std::vector<Edgel> edgels;

	for (size_t y = 0; y < regionHeight; y += STEP_SIZE) {
		unsigned char* pixelPosition = buffer->getData() + (regionLeft + (y + regionTop) * width) * NUMBER_OF_CHANNELS;
		previousPixelOne = previousPixelTwo = 0;
		const size_t pitch = NUMBER_OF_CHANNELS;

		for (size_t x = 0; x < regionWidth; x++) {
			++numberOfIterations;
			unsigned int firstChannel = edgeKernel(pixelPosition, pitch);
			if (firstChannel > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 1, pitch) > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 2, pitch) > KERNEL_THRESHOLD) {
			} else {
				firstChannel = 0;
			}

			if (previousPixelOne > 0 && previousPixelOne > previousPixelTwo && previousPixelOne > firstChannel) {
				size_t trueX = regionLeft + x;
				size_t trueY = regionTop + y;
				edgels.emplace_back(Edgel(trueX, trueY, edgelGradientIntensity(trueX, trueY)));
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
			++numberOfIterations;
			unsigned int firstChannel = edgeKernel(pixelPosition, pitch);
			if (firstChannel > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 1, pitch) > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 2, pitch) > KERNEL_THRESHOLD) {
			} else {
				firstChannel = 0;
			}

			if (previousPixelOne > 0 && previousPixelOne > previousPixelTwo && previousPixelOne > firstChannel) {
				size_t trueX = regionLeft + x;
				size_t trueY = regionTop + y;
				edgels.emplace_back(Edgel(trueX, trueY, edgelGradientIntensity(trueX, trueY)));
			}
			previousPixelTwo = previousPixelOne;
			previousPixelOne = firstChannel;

			pixelPosition += pitch;
		}
	}

	return edgels;
}

std::vector<Edgel> EdgelDetector::iterateOverDimensions(size_t regionLeft, size_t regionTop, size_t firstLimit, size_t secondLimit, size_t pitch,
	std::function<size_t()> pixelPositionFunction, std::function<Edgel()> createEdgel) {
	
	std::vector<Edgel> edgels;

	for (size_t firstDimension = 0; firstDimension < firstLimit; firstDimension += STEP_SIZE) {
		size_t width = buffer->getWidth();
		unsigned int previousPixelOne = 0, previousPixelTwo = 0;
		unsigned char* pixelPosition = buffer->getData() + pixelPositionFunction();

		for (size_t secondDimension = 0; secondDimension < secondLimit; secondDimension += STEP_SIZE) {
			unsigned int firstChannel = edgeKernel(pixelPosition, pitch);
			if (firstChannel > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 1, pitch) > KERNEL_THRESHOLD &&
				edgeKernel(pixelPosition + 2, pitch) > KERNEL_THRESHOLD) {
			} else {
				firstChannel = 0;
			}

			if (previousPixelOne > 0 && previousPixelOne > previousPixelTwo && previousPixelOne > firstChannel) {
				edgels.emplace_back(createEdgel());
			}
			previousPixelTwo = previousPixelOne;
			previousPixelOne = firstChannel;

			pixelPosition += pitch;
		}
	}

	return std::vector<Edgel>();
}



unsigned int EdgelDetector::edgeKernel(unsigned char* offset, size_t pitch) const {
	int kernel = -3 * offset[-2 * pitch];
	kernel += -5 * offset[-pitch];
	kernel += 5 * offset[pitch];
	kernel += 3 * offset[2 * pitch];

	return abs(kernel);
}

Vector2f EdgelDetector::edgelGradientIntensity(size_t x, size_t y) const {
	int gradientX = buffer->getPixel(x - 1, y - 1);
	gradientX += 2 * buffer->getPixel(x, y - 1);
	gradientX += buffer->getPixel(x + 1, y - 1);
	gradientX -= buffer->getPixel(x - 1, y + 1);
	gradientX -= 2 * buffer->getPixel(x, y + 1);
	gradientX -= buffer->getPixel(x + 1, y + 1);

	int gradientY = buffer->getPixel(x - 1, y - 1);
	gradientY += 2 * buffer->getPixel(x - 1, y);
	gradientY += buffer->getPixel(x - 1, y + 1);
	gradientY -= buffer->getPixel(x + 1, y - 1);
	gradientY -= 2 * buffer->getPixel(x + 1, y);
	gradientY -= buffer->getPixel(x + 1, y + 1);

	return Vector2f(static_cast<float>(gradientX), static_cast<float>(gradientY)).get_normalized();
}
