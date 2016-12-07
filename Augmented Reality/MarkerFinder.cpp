#include "MarkerFinder.h"


std::vector<ARMarker> MarkerFinder::findMarkers(std::vector<LineSegment> linesWithCorners) {
	std::vector<ARMarker> markers;
	std::list<LineSegment> listOfLines{ linesWithCorners.begin(), linesWithCorners.end() };

	while (listOfLines.size() >= 4) {
		markers.push_back(findMarker(listOfLines));
	}

	return markers;
}

ARMarker MarkerFinder::findMarker(std::list<LineSegment>& listOfSegments) {
	size_t lengthOfChain = 0;
	std::vector<LineSegment> chain;

	bool nextFound = false;

	do {
		auto& startOfChain = listOfSegments.front();
		listOfSegments.pop_front();

		for (auto& segment : listOfSegments) {

		}
	} while (nextFound && chain.size() < 4);

	return ARMarker();
}

bool MarkerFinder::areSegmentsInChain(LineSegment & predecessor, LineSegment & successor) {
	return false;
}
