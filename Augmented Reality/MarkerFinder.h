#pragma once
#include <list>

#include "LineSegment.h"
#include "ARMarker.h"

const float CHAIN_MAX_GAP = 16.0f;

enum class EndPoint {NONE, START, END};

class MarkerFinder {
	
	//std::vector<LineSegment> findMarker(std::list<LineSegment>& listOfSegments);
	ARMarker convertChainToARMarker(std::vector<LineSegment>& chain);
	
	void findChainOfLines(LineSegment &startSegment, bool atStartPoint, std::vector<LineSegment> &linesegments, std::vector<LineSegment> &chain, int& length);
	bool areSegmentsInChain(const LineSegment& first, const LineSegment& second, bool atStartPoint) const;


public:
	std::vector<ARMarker> findMarkers(std::vector<LineSegment> linesWithCorners);
};

