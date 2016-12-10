#include "MarkerFinder.h"

/*
std::vector<ARMarker> MarkerFinder::findMarkers(std::vector<LineSegment> linesWithCorners) {
	std::vector<ARMarker> markers;
	std::vector<std::vector<LineSegment>> chains;
	std::list<LineSegment> listOfLines{ linesWithCorners.begin(), linesWithCorners.end() };

	while (listOfLines.size() >= 4) {
		auto chain = findMarker(listOfLines);
		if (chain.size() == 4) {
			markers.push_back(convertChainToARMarker(chain));
		}
	}

	return markers;
}

std::vector<LineSegment> MarkerFinder::findMarker(std::list<LineSegment>& listOfSegments) {
	size_t lengthOfChain = 0;
	std::vector<LineSegment> chain;

	auto startOfChain = listOfSegments.front();
	listOfSegments.pop_front();
	chain.push_back(LineSegment(startOfChain));

	bool nextFound = false;
	bool isStart = true;
	EndPoint endPoint = EndPoint::NONE;

	do {
		nextFound = false;
		isStart = !(chain.size() > 1);

		for (auto& segment : listOfSegments) {
			auto isChain = areSegmentsInChain(startOfChain, segment, isStart, endPoint);
			if (isChain.first) {
				chain.push_back(LineSegment(segment));
				startOfChain = chain[chain.size() - 1];
				listOfSegments.remove(segment);
				nextFound = true;
				endPoint = isChain.second;
				break;
			}
		}
	} while (nextFound && chain.size() < 4);

	return chain;
}
*/

ARMarker MarkerFinder::convertChainToARMarker(std::vector<LineSegment>& chain) {
	size_t size = chain.size();

	std::array<Vector2f, 4> vertices;

	for (size_t i = 0; i < size; i++) {
		auto& predecessor = chain[i % size];
		auto& successor = chain[(i + 1) % size];

		auto intersectionPoint = predecessor.getIntersection(successor);
		vertices[i] = intersectionPoint;
	}

	return ARMarker(vertices);
}


std::vector<ARMarker> MarkerFinder::findMarkers(std::vector<LineSegment> linesWithCorners) {
	std::vector<ARMarker> markers;

	while (linesWithCorners.size() >= 4) {
		auto chain = findChainOfLines(linesWithCorners);

		if (chain.size() == 4) {
			markers.push_back(convertChainToARMarker(chain));
		}
	}

	return markers;
}

std::vector<LineSegment> MarkerFinder::findChainOfLines(std::vector<LineSegment>& lineSegments) {
	LineSegment startSegment = lineSegments[0];
	lineSegments[0] = lineSegments[lineSegments.size() - 1];
	lineSegments.resize(lineSegments.size() - 1);

	std::vector<LineSegment> chain;
	chain.push_back(startSegment);

	bool nextSegmentFound = false;

	do {
		nextSegmentFound = false;

		for (auto it = lineSegments.begin(); it != lineSegments.end(); ++it) {
			if (areSegmentsInChain(startSegment, (*it))) {
				LineSegment chainSegment = (*it);
				lineSegments.erase(it);

				chain.push_back(chainSegment);
				startSegment = chainSegment;

				nextSegmentFound = true;
				break;
			}
		}
	} while (nextSegmentFound && chain.size() < 4);

	return chain;
}

bool MarkerFinder::areSegmentsInChain(const LineSegment& first, const LineSegment& second) const {
	if (first.isOrientationCompatible(second)) {
		return false;
	}

	const auto& startPoint = first.start.position;
	const auto& endPoint = second.end.position;

	if ((endPoint - startPoint).get_squared_length() > CHAIN_MAX_GAP) {
		return false;
	}

	const float orientation = first.slope.x * second.slope.y - first.slope.y * second.slope.x;

	if (orientation <= 0) {
		return false;
	}

	return true;
}
