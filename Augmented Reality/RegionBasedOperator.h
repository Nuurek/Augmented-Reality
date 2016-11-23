#pragma once


class RegionBasedOperator {
protected:
	const size_t BORDER_SIZE;
	const size_t REGION_SIZE;
	const size_t STEP_SIZE;

public:
	RegionBasedOperator(const size_t borderSize, const size_t regionSize, const size_t stepSize);
};

