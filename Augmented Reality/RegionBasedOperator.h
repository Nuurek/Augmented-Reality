#pragma once
#include "Buffer.h"

class RegionBasedOperator {
protected:
	const size_t BORDER_SIZE;
	const size_t REGION_SIZE;
	const size_t STEP_SIZE;

	Buffer* buffer;

	inline bool isPointOutsideBorder(float x, float y) const {
		return (
			x < BORDER_SIZE ||
			x > buffer->getWidth() - BORDER_SIZE ||
			y < BORDER_SIZE ||
			y > buffer->getHeight() - BORDER_SIZE
			);
	}
	inline bool isPointInsideFrame(size_t x, size_t y) const {
		return (
			x >= 0 &&
			x < buffer->getWidth() &&
			y >= 0 &&
			y < buffer->getHeight()
			);
	}

public:
	RegionBasedOperator(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	void setBuffer(Buffer* buffer);
};

