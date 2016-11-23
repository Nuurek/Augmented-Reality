#pragma once
#include "Buffer.h"

class RegionBasedOperator {
protected:
	const size_t BORDER_SIZE;
	const size_t REGION_SIZE;
	const size_t STEP_SIZE;

	Buffer* buffer;

public:
	RegionBasedOperator(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	void setBuffer(Buffer* buffer);
};

