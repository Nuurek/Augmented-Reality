#pragma once
#include <vector>
#include <algorithm>
#include <chrono>
#include <functional>
#include "Buffer.h"
#include "Edgel.h"


const std::vector<int> GAUSSIAN_COEFFICIENTS = { -5, -3, 0, 3, 5 };
const size_t BORDER_SIZE = GAUSSIAN_COEFFICIENTS.size() / 2;
const size_t REGION_SIZE = 40;
const size_t NUMBER_OF_CHANNELS = 3;
const size_t STEP_SIZE = 5;
const size_t KERNEL_THRESHOLD = 255;

class EdgelDetector {
	Buffer* buffer;

	static const std::vector<float> gaussianCoefficients;
	static const size_t borderSize;
	static const size_t regionSize;

	std::vector<Edgel> findEdgelsInRegion(size_t regionLeft, size_t regionTop, size_t regionWidth, size_t regionHeight);
	std::vector<Edgel> iterateOverDimensions(size_t regionLeft, size_t regionTop, size_t firstLimit, size_t secondLimit, size_t pitch,
		std::function<size_t()> pixelPosition, std::function<Edgel()> createEdgel);
	unsigned int edgeKernel(unsigned char* offset, size_t pitch) const;
	Vector2f edgelGradientIntensity(size_t x, size_t y) const;

	long long numberOfIterations;

public:
	void setBuffer(Buffer* buffer);

	std::vector<Edgel> findEdgels();
};