#pragma once
#include "RegionBasedOperator.h"
#include "Edgel.h"
#include "EdgelDetector.h"
#include "LineSegment.h"
#include "LineSegmentDetector.h"

const size_t EDGELS_IN_REGION = 5;

class ARMarkerDetector : public RegionBasedOperator {
	EdgelDetector edgelDetector;
	std::vector<Edgel> edgels;
	LineSegmentDetector lineSegmentDetector;
	std::vector<LineSegment> lineSegments;

	void clearStructures();

public:
	ARMarkerDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	
	void setBuffer(Buffer* buffer);

	void findARMarkers();

	std::vector<Edgel> getEdgels();
	std::vector<LineSegment> getLineSegments();
};

