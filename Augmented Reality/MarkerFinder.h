#pragma once
#include <list>

#include "LineSegment.h"
#include "ARMarker.h"

const float CHAIN_MAX_GAP = 64.0f;

enum class EndPoint {NONE, START, END};

class MarkerFinder {
	
	std::vector<LineSegment> findMarker(std::list<LineSegment>& listOfSegments);
	std::pair<bool, EndPoint> areSegmentsInChain(LineSegment& predecessor, LineSegment& successor, bool isLastInChain, EndPoint endPoint);
	ARMarker convertChainToARMarker(std::vector<LineSegment>& chain);
	
	//void findChainOfLines(LineSegment &startSegment, bool atStartPoint, std::vector<LineSegment> &linesegments, std::vector<LineSegment> &chain, int& length);

public:
	std::vector<ARMarker> findMarkers(std::vector<LineSegment> linesWithCorners);
};

