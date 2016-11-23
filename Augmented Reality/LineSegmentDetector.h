#pragma once
#include "RegionBasedOperator.h"
#include "Edgel.h"

class LineSegmentDetector : public RegionBasedOperator {

public:
	LineSegmentDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize);
};
