#pragma once
#include <vector>
#include <algorithm>
#include <chrono>
#include <functional>
#include "Buffer.h"
#include "Edgel.h"


const size_t NUMBER_OF_CHANNELS = 3;
const size_t KERNEL_THRESHOLD = 255;

class EdgelDetector {
	Buffer* buffer;

	const size_t BORDER_SIZE;
	const size_t REGION_SIZE;
	const size_t STEP_SIZE;

	std::vector<Edgel> findEdgelsInRegion(size_t regionLeft, size_t regionTop, size_t regionWidth, size_t regionHeight);
	std::vector<Edgel> iterateOverDimensions(size_t regionLeft, size_t regionTop, size_t firstLimit, size_t secondLimit, size_t pitch,
		std::function<size_t()> pixelPosition, std::function<Edgel()> createEdgel);
	unsigned int edgeKernel(unsigned char* offset, size_t pitch) const;
	Vector2f edgelGradientIntensity(size_t x, size_t y) const;

	long long numberOfIterations;

public:
	EdgelDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	void setBuffer(Buffer* buffer);

	std::vector<Edgel> findEdgels();
};