#pragma once
#include <vector>
#include <algorithm>
#include <chrono>
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

	std::vector<Edgel> findEdgelsInRegion(size_t regionLeft, size_t regionTop, size_t regionRight, size_t regionBottom);
	unsigned int edgeKernel(unsigned char* offset, size_t pitch) const;

public:
	void setBuffer(Buffer* buffer);

	std::vector<Edgel> findEdgels();
};