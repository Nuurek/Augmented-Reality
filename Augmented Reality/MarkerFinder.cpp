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
		nextFound = false;

		auto startOfChain = listOfSegments.front();
		listOfSegments.pop_front();
		chain.push_back(LineSegment(startOfChain));

		for (auto& segment : listOfSegments) {
			if (areSegmentsInChain(startOfChain, segment, false)) {
				chain.push_back(LineSegment(segment));
				listOfSegments.remove(segment);
				nextFound = true;
			}
		}
	} while (nextFound && chain.size() < 4);

	return ARMarker();
}

bool MarkerFinder::areSegmentsInChain(LineSegment & predecessor, LineSegment & successor, bool isLastInChain) {
	// check if not parallel
	if (predecessor.isOrientationCompatible(successor)) {
		return false;
	}

	// check if not to far away
	auto predecessorPoint = isLastInChain ? predecessor.end.position : predecessor.start.position;
	auto successorPoint = isLastInChain ? successor.start.position : successor.end.position;

	if ((predecessorPoint - successorPoint).get_squared_length() > CHAIN_MAX_GAP) {
		return false;
	}

	// check if orientation is counterclockwise
	float orientation = predecessor.slope.x * successor.slope.y - predecessor.slope.y * successor.slope.x;
	if (isLastInChain) {
		orientation *= -1.0f;
	}

	if (orientation <= 0.0f) {
		return false;
	}

	return true;
}
