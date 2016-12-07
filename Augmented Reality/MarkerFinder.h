#pragma once
#include <list>

#include "LineSegment.h"
#include "ARMarker.h"

class MarkerFinder {
	ARMarker findMarker(std::list<LineSegment>& listOfSegments);
	bool areSegmentsInChain(LineSegment& predecessor, LineSegment& successor);

public:
	std::vector<ARMarker> findMarkers(std::vector<LineSegment> linesWithCorners);
};

