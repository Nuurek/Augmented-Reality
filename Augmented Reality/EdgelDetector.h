#pragma once
#include <vector>
#include <algorithm>
#include <chrono>
#include <functional>
#include "RegionBasedOperator.h"
#include "Buffer.h"
#include "Edgel.h"


const size_t NUMBER_OF_CHANNELS = 3;
const size_t KERNEL_THRESHOLD = 255;

class EdgelDetector : public RegionBasedOperator {
protected:
	std::vector<Edgel> iterateOverDimensions(size_t regionLeft, size_t regionTop, size_t firstLimit, size_t secondLimit, size_t pitch,
		std::function<size_t()> pixelPosition, std::function<Edgel()> createEdgel);
	unsigned char* calcualteOffset(size_t x, size_t y) const;
	unsigned int edgeKernel(unsigned char* offset, size_t pitch) const;
	unsigned int edgeKernelX(size_t x, size_t y) const ;
	unsigned int edgeKernelY(size_t x, size_t y) const;
	inline bool isPointInFrame(float x, float y) const {
		return (
			x < BORDER_SIZE ||
			x > buffer->getWidth() - BORDER_SIZE ||
			y < BORDER_SIZE ||
			y > buffer->getHeight() - BORDER_SIZE
			);
	}
	Vector2f edgelGradientIntensity(size_t x, size_t y) const;

	long long numberOfIterations;

public:
	EdgelDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	std::vector<Edgel> findEdgelsInRegion(size_t regionLeft, size_t regionTop, size_t regionWidth, size_t regionHeight);
};