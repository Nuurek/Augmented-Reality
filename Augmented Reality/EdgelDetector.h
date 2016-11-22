#pragma once
#include <vector>
#include <algorithm>
#include "Buffer.h"
#include "Edgel.h"


const std::vector<float> GAUSSIAN_COEFFICIENTS = { -5.0, -3.0, 0.0, 3.0, 5.0 };
const size_t BORDER_SIZE = GAUSSIAN_COEFFICIENTS.size() / 2;
const size_t REGION_SIZE = 40;


class EdgelDetector {
	Buffer* buffer;

	static const std::vector<float> gaussianCoefficients;
	static const size_t borderSize;
	static const size_t regionSize;

	std::vector<Edgel> findEdgelsInRegion(size_t borderLeft, size_t borderTop, size_t borderRight, size_t borderBottom);

public:
	void setBuffer(Buffer* buffer);

	std::vector<Edgel> findEdgels();
};