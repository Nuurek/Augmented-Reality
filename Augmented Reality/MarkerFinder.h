#pragma once
#include <list>

#include "LineSegment.h"
#include "ARMarker.h"

const float CHAIN_MAX_GAP = 16.0f;

class MarkerFinder {
	ARMarker findMarker(std::list<LineSegment>& listOfSegments);
	bool areSegmentsInChain(LineSegment& predecessor, LineSegment& successor, bool isLastInChain);

public:
	std::vector<ARMarker> findMarkers(std::vector<LineSegment> linesWithCorners);
};

