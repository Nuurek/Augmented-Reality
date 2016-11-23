#include "LineSegmentDetector.h"

LineSegmentDetector::LineSegmentDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize) 
	: RegionBasedOperator(borderSize, regionSize, stepSize) {}
