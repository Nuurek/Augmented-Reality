#pragma once
#include <list>

#include "LineSegment.h"
#include "ARMarker.h"

const float CHAIN_MAX_GAP = 16.0f;

enum class EndPoint {NONE, START, END};

class MarkerFinder {
	
	//std::vector<LineSegment> findMarker(std::list<LineSegment>& listOfSegments);
	ARMarker convertChainToARMarker(std::vector<LineSegment>& chain);
	
	std::vector<LineSegment> findChainOfLines(std::vector<LineSegment> &linesegments);
	bool areSegmentsInChain(const LineSegment& first, const LineSegment& second) const;


public:
	std::vector<ARMarker> findMarkers(std::vector<LineSegment> linesWithCorners);
};

